# MFC_and_Coroutines

```C++
FireAndForget CMFCandCoroutinesDlg::TestCoroutine()
{
	auto print_thread_id{ [this](std::wstring_view msg) {
		AfxMessageBox(std::format(L"{}\nThread ID: {}\n", msg, std::this_thread::get_id()).c_str());
	} };

	print_thread_id(L"Start a coroutine.");

	co_await ResumeBackground();

	print_thread_id(L"Running in the backgound for 5 seconds.");

	using namespace std::chrono_literals;

	std::this_thread::sleep_for(5s);

	co_await ResumeMfcForeground(this, WM_USER_COROUTINE);

	print_thread_id(L"End of coroutine.");
}
```
