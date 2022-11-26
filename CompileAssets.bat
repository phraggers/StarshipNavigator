if not exist build\x64_Debug\data\ mkdir build\x64_Debug\data\
if not exist build\x64_Release\data\ mkdir build\x64_Release\data\

if exist build\x64_Debug\data\main.dat del build\x64_Debug\data\main.dat
if exist build\x64_Release\data\main.dat del build\x64_Release\data\main.dat

pushd assets
7z a ../build/x64_Debug/data/main.dat main/*
7z a ../build/x64_Release/data/main.dat main/*
popd