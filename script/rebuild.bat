:: Importing key value pairs from https://stackoverflow.com/questions/6832496/how-to-add-a-set-path-only-for-that-batch-file-executing
IF EXIST env.txt FOR /F "delims=" %%A IN (env.txt) DO SET "%%A"

rmdir /s /q build
"%CMAKE_PATH%\cmake.exe" -B build -S . --preset=default
ninja -C build