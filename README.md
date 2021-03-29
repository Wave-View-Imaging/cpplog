# C++ Log
This library aims to provide a simple thread-safe interface for logging in C++.

1. [Installation](#installation)
2. [Basic usage](#basic-usage)
3. [Severity Levels](#severity-levels)
4. [Logging Backends](#logging-backends)
5. [Adding Observers](#adding-observers)

## Installation
Adding this library to a project can be done by installing a git submodule to your project. 
```bash
git submodule add https://github.com/Wave-View-Imaging/cpplog.git log
```

Include this as a new library in cmake by adding the following to your relevant CMakeLists.txt files.
```cmake
...
add_subdirectory(log)
...
target_link_libraries(${your_target} ... log)
```

Include `log.h` to your source files to start using the library.
```cpp
#include <log.h>
```

## Basic Usage
```cpp
#include <log.h>

int main(int argc, char* argv[]) {
	Log::AddFileBackend("debugfilename.log", Log::Severity::Warn | Log::Severity::Debug);
	Log::AddConsoleBackend([](const Log::Severity& severity) {
		// Log everything to console
		return true;
	});

    std::string myVar{"not found."};
	Log::Debug("The value of myVar is {}.", myVar);
	Log::Warn("Something is definitely not right...");
	...
```
## Severity Levels
| Severity      | Description   |
| :------------ |:--------------|
| `Trace`      | Useful for pinpointing the location in the code |
| `Debug`      | Used for diagnostic purposes, can potentially be useful outside of development       |
| `Info`      | Generally useful information to note (start/stop, configurations used, etc..)      |
| `Warn`      | Used to log potential problem areas or to note potentially problematic assumptions made      |
| `Error`      | Used to log fatal errors to an *operation*, but not to the application (problems communicating with I/O, etc..)      |
| `Fatal`      | Useful for logging anything that will force the shutdown of the application      |

## Logging Backends
This library supports adding multiple backends for the logs to be send to. The following example shows how logs can be split into two different file backends.
```cpp
...
Log::AddFileBackend("debugfilename.log", Log::Severity::Trace);
Log::AddFileBackend("error_filename.log", Log::Severity::Error | Log::Severity::Fatal);
...
```
Adding the same backend multiple time doesn't have negative effects; in the following example, the 
```cpp
...
Log::AddFileBackend("filename.log", Log::Severity::Debug);
Log::AddFileBackend("filename.log", Log::Severity::Error);
...
```
The following example shows how backends can be removed. Note however that all buffered data will be written out to the backend before removing it from future use.
```cpp
...
Log::RemoveFileBackend("filename.log");
...
```
Finally, note that `Log::Backend::Console` is the default backend and does not need to be added; this backend will be send messages of all severity levels by default.

## Adding Observers
It is possible that application code might want to be notified of the messages being sent for logging. One potential usecase for this might be with showing messages to the user in a GUI application. The following example shows how a function can be added as an observer to messages to the logging system.
```cpp
...
Log::AddObserver(
	Log::Severity::Warn | Log::Severity::Error, 
	[](const Log::Severity& severity) {
		if (severity == Log::Severity::Warn) {
			showWarningMessage();
		} else {
			showErrorMessage();
		}
	}
);
...
```
