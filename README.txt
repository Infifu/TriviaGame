Due to the usage of the curl external library the code might not compile if
You do not have the MFC installed in your visual studio
To fix this:
1.Open visual studio installer
2.Modify your installed version
3.Go to Individual Components
4.Search for MFC (C++ MFC for v143 build tools (x86 & x64))
5.Click Modify to install

OR

You do not have the curl files included in your project
The error <curl/curl.h> is not found

Include Path
Go to: Project -> Properties -> C/C++ -> General -> Additional Include Directories
Add: curl/include

Library Path
Go to: Project -> Properties -> Linker -> General -> Additional Library Directories
Add: curl

Link the Library
Go to: Project -> Properties -> Linker -> Input -> Additional Dependencies
Add: libcurl_a.lib (or libcurl_a_debug.lib in Debug mode)

Define Static Link
Go to: Project -> Properties -> C/C++ -> Preprocessor -> Preprocessor Definitions
Add: CURL_STATICLIB

Amen.