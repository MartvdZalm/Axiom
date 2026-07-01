#include <axiom/lexer/lexer.hpp>
#include <cctype>
#include <unordered_map>

namespace axiom::lexer
{

namespace
{

bool is_identifier_start(unsigned char c)
{
    return std::isalpha(c) != 0 || c == '_';
}

bool is_identifier_continue(unsigned char c)
{
    return std::isalnum(c) != 0 || c == '_';
}

/// Minimal UTF-8 lead-byte check for advancing one code point on invalid sequences.
bool is_utf8_continuation(unsigned char c)
{
    return (c & 0xC0) == 0x80;
}

} // namespace

Lexer::Lexer(const core::SourceManager& sources, std::uint32_t buffer_id)
    : sources_(sources), buffer_id_(buffer_id), source_(sources.text(buffer_id))
{
}

char Lexer::current() const
{
    if (pos_ >= source_.size())
    {
        return '\0';
    }
    return source_[pos_];
}

char Lexer::peek_char(std::size_t ahead) const
{
    const auto index = pos_ + ahead;
    if (index >= source_.size())
    {
        return '\0';
    }
    return source_[index];
}

void Lexer::advance()
{
    if (pos_ >= source_.size())
    {
        return;
    }

    const unsigned char ch = static_cast<unsigned char>(source_[pos_]);
    if (ch == '\n')
    {
        ++line_;
        column_ = 1;
    }
    else
    {
        ++column_;
    }
    ++pos_;

    // Skip UTF-8 continuation bytes after consuming a lead byte.
    while (pos_ < source_.size() && is_utf8_continuation(static_cast<unsigned char>(source_[pos_])))
    {
        ++pos_;
    }
}

void Lexer::skip_whitespace_and_comments()
{
    for (;;)
    {
        const unsigned char ch = static_cast<unsigned char>(current());
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        {
            advance();
            continue;
        }
        if (ch == '/' && peek_char() == '/')
        {
            while (current() != '\0' && current() != '\n')
            {
                advance();
            }
            continue;
        }
        break;
    }
}

core::SourceLocation Lexer::location_at(std::size_t offset) const
{
    std::uint32_t line = 1;
    std::uint32_t column = 1;
    for (std::size_t i = 0; i < offset && i < source_.size(); ++i)
    {
        if (source_[i] == '\n')
        {
            ++line;
            column = 1;
        }
        else if ((static_cast<unsigned char>(source_[i]) & 0xC0) != 0x80)
        {
            ++column;
        }
    }
    return core::SourceLocation{
        static_cast<std::uint32_t>(offset),
        line,
        column,
    };
}

Token Lexer::make_token(TokenKind kind, std::size_t start, std::size_t end)
{
    const auto begin = location_at(start);
    const auto end_loc = location_at(end);
    return Token{
        kind,
        core::SourceSpan{begin, end_loc},
        source_.substr(start, end - start),
    };
}

TokenKind Lexer::keyword_kind(std::string_view text) const
{
    static const std::unordered_map<std::string_view, TokenKind> keywords = {
        {"machine", TokenKind::MachineKw},
        {"state", TokenKind::StateKw},
        {"transition", TokenKind::TransitionKw},
        {"on", TokenKind::OnKw},
    };
    const auto it = keywords.find(text);
    if (it == keywords.end())
    {
        return TokenKind::Identifier;
    }
    return it->second;
}

Token Lexer::lex_identifier_or_keyword(std::size_t start)
{
    while (is_identifier_continue(static_cast<unsigned char>(current())))
    {
        advance();
    }
    const auto end = pos_;
    const auto text = source_.substr(start, end - start);
    const auto kind = keyword_kind(text);
    return make_token(kind, start, end);
}

Token Lexer::next()
{
    if (has_peek_)
    {
        has_peek_ = false;
        return peeked_;
    }

    skip_whitespace_and_comments();
    const std::size_t start = pos_;

    const unsigned char ch = static_cast<unsigned char>(current());
    if (ch == '\0')
    {
        return make_token(TokenKind::EndOfFile, start, start);
    }

    if (is_identifier_start(ch))
    {
        advance();
        return lex_identifier_or_keyword(start);
    }

    if (ch == '{')
    {
        advance();
        return make_token(TokenKind::LBrace, start, pos_);
    }
    if (ch == '}')
    {
        advance();
        return make_token(TokenKind::RBrace, start, pos_);
    }
    if (ch == '-' && peek_char() == '>')
    {
        advance();
        advance();
        return make_token(TokenKind::Arrow, start, pos_);
    }

    diagnostics_.add_error(buffer_id_, location_at(start),
                           std::string("unexpected character '") + static_cast<char>(ch) + "'");
    advance();
    return make_token(TokenKind::Invalid, start, pos_);
}

Token Lexer::peek()
{
    if (!has_peek_)
    {
        peeked_ = next();
        has_peek_ = true;
    }
    return peeked_;
}

void Lexer::consume()
{
    if (has_peek_)
    {
        has_peek_ = false;
    }
    else
    {
        (void)next();
    }
}

} // namespace axiom::lexer
