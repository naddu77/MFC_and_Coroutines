#pragma once

#include <threadpoolapiset.h>
#include <errhandlingapi.h>
#include <coroutine>
#include <cstdlib>
#include <stdexcept>
#include <format>

struct FireAndForget
{
	// Nothing
};

template <typename... Ts>
struct std::coroutine_traits<FireAndForget, Ts...>
{
	struct promise_type
	{
		FireAndForget get_return_object() const noexcept
		{
			return {};
		}

		void return_void() const noexcept
		{

		}

		std::suspend_never initial_suspend() const noexcept
		{
			return {};
		}

		std::suspend_never final_suspend() const noexcept
		{
			return {};
		}

		void unhandled_exception() const noexcept
		{
			std::abort();
		}
	};
};

[[nodiscard]] inline auto ResumeBackground() noexcept
{
	struct Awaitable
	{
		bool await_ready() const noexcept
		{
			return false;
		}

		void await_resume() const noexcept
		{
			// Nothing
		}

		void await_suspend(std::coroutine_handle<> coro) const
		{
			if (!TrySubmitThreadpoolCallback(&ResumeBackgroundCallback, coro.address(), nullptr))
			{
				std::error_code ec{ static_cast<int>(GetLastError()), std::system_category() };

				throw std::system_error{ ec };
			}
		}

		inline static void __stdcall ResumeBackgroundCallback(PTP_CALLBACK_INSTANCE, PVOID context) noexcept
		{
			std::coroutine_handle<>::from_address(context)();
		}
	};

	return Awaitable{};
}

template <typename T>
concept MfcDialog = requires (T* t)
{
	{ t->PostMessageW(0, 0) };
};

template <MfcDialog CWND>
requires std::is_same_v<UINT, unsigned int>
[[nodiscard]] inline auto ResumeMfcForeground(CWND* dlg, unsigned int message_id) noexcept
{
	struct Awaitable
	{
		Awaitable(CWND* dlg, unsigned int message_id)
			: dlg{ dlg }
			, message_id{ message_id }
		{
			// Nothing
		}

		bool await_ready() const noexcept
		{
			return false;
		}

		void await_resume() const noexcept
		{
			// Nothing
		}

		void await_suspend(std::coroutine_handle<> handle) const
		{
			dlg->PostMessageW(message_id, reinterpret_cast<WPARAM>(handle.address()));
		}

	private:
		CWND* dlg;
		unsigned int message_id;
	};

	return Awaitable{ dlg, message_id };
}