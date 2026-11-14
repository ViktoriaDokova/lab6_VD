#pragma once

#include <coroutine>
#include <utility>

// Owns a coroutine handle and destroys the coroutine when it goes out of scope.
// The coroutine starts suspended and is started by an explicit resume().
class Coroutine {
public:
    struct promise_type {
        Coroutine get_return_object() { return Coroutine(Handle::from_promise(*this)); }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { throw; }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Coroutine(Handle handle) : handle_(handle) {}
    Coroutine(Coroutine&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;
    Coroutine& operator=(Coroutine&&) = delete;

    ~Coroutine() {
        if (handle_)
            handle_.destroy();
    }

    std::coroutine_handle<> handle() const { return handle_; }
    void resume() { handle_.resume(); }

private:
    Handle handle_;
};
