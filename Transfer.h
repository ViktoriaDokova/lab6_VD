#pragma once

#include <coroutine>

// Shared state between the generator and the two printing coroutines.
struct Exchange {
    int value = 0;
    std::coroutine_handle<> generator;
    std::coroutine_handle<> evenPrinter;  // coroutine A
    std::coroutine_handle<> oddPrinter;   // coroutine B
};

// Awaited by the generator: stores the number and passes control
// to A (even number) or B (odd number).
class SendByParity {
public:
    SendByParity(Exchange& exchange, int value) : exchange_(exchange), value_(value) {}

    bool await_ready() const noexcept { return false; }

    // Returning a handle resumes that coroutine directly (symmetric transfer).
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> self) noexcept {
        exchange_.value = value_;
        exchange_.generator = self;
        return value_ % 2 == 0 ? exchange_.evenPrinter : exchange_.oddPrinter;
    }

    void await_resume() const noexcept {}

private:
    Exchange& exchange_;
    int value_;
};

// Awaited by A and B: passes control back to the generator.
class ReturnToGenerator {
public:
    explicit ReturnToGenerator(Exchange& exchange) : exchange_(exchange) {}

    bool await_ready() const noexcept { return false; }

    std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept {
        return exchange_.generator;
    }

    void await_resume() const noexcept {}

private:
    Exchange& exchange_;
};
