#pragma once

/// @file result.hpp
/// @brief Exception-free success/failure wrapper for compiler pipeline stages.

#include <axiom/core/diagnostic.hpp>

#include <optional>
#include <utility>

namespace axiom::core {

/// @brief Carries a value on success or a @ref DiagnosticBag on failure.
///
/// Replaces exceptions in the lexer/parser/semantic/codegen pipeline. Each stage
/// returns `Result<T>` so callers can short-circuit on failure while preserving
/// rich diagnostics for the CLI.
///
/// Success requires both a stored value and no error-level diagnostics in the bag.
/// Warnings may be present without failing the stage.
///
/// @tparam T Payload type (e.g. @ref axiom::ast::TranslationUnit).
template<typename T>
class Result {
public:
    /// @brief Constructs a successful result owning @p value.
    static Result ok(T value) {
        Result r;
        r.value_ = std::move(value);
        return r;
    }

    /// @brief Constructs a failed result; @p diagnostics explains why.
    static Result fail(DiagnosticBag diagnostics) {
        Result r;
        r.diagnostics_ = std::move(diagnostics);
        return r;
    }

    /// @brief True when a value is present and the diagnostic bag has no errors.
    [[nodiscard]] bool succeeded() const noexcept {
        return value_.has_value() && !diagnostics_.has_errors();
    }

    [[nodiscard]] explicit operator bool() const noexcept { return succeeded(); }

    [[nodiscard]] const T& value() const& { return value_.value(); }
    [[nodiscard]] T& value() & { return value_.value(); }
    [[nodiscard]] T&& value() && { return std::move(value_.value()); }

    [[nodiscard]] const DiagnosticBag& diagnostics() const noexcept { return diagnostics_; }
    [[nodiscard]] DiagnosticBag& diagnostics() noexcept { return diagnostics_; }

    /// @brief Incorporates diagnostics from a sub-stage (e.g. nested parse).
    void merge_diagnostics(const DiagnosticBag& other) { diagnostics_.merge(other); }

private:
    std::optional<T> value_;
    DiagnosticBag diagnostics_;
};

} // namespace axiom::core
