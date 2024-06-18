# MFC_and_Coroutines
This is an example of applying a C++20 coroutine to MFC `CWnd`.

The usage example is shown below.

```C++
FireAndForget CMFCandCoroutinesDlg::TestCoroutine()
{
	auto print_thread_id{ [this](std::wstring_view msg) {
		std::wstringstream ss;

		ss << std::this_thread::get_id();

		AfxMessageBox(std::format(L"{}\nThread ID: {}\n", msg, ss.str()).c_str());
	} };

	print_thread_id(L"Start a coroutine.");

	co_await ResumeBackground();

	print_thread_id(L"Running in the background for 5 seconds.");

	using namespace std::chrono_literals;

	std::this_thread::sleep_for(5s);

	co_await ResumeMfcForeground(this, WM_USER_COROUTINE);

	print_thread_id(L"End of coroutine.");
}

```
