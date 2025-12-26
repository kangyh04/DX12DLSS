# DX12DLSS
DX12 프로젝트에서 DLSS사용 프로젝트

## 🛠 Streamline 샘플 빌드 오류(dxcompiler.dll) 해결 가이드
1. 문제의 원인
ShaderMake 도구가 셰이더를 컴파일할 때 Windows SDK 내부의 dxcompiler.dll과 dxil.dll을 참조해야 함.

하지만 Visual Studio의 자동 빌드 스크립트가 .../bin/10.0.xxxxx.0/x64/ 경로를 제대로 인식하지 못해 "Failed to load compiler dll" 에러와 함께 setlocal 오류 발생.

2. 해결 단계 1: 셰이더 수동 컴파일 (PowerShell)
Visual Studio가 경로를 못 찾으므로, 우리가 직접 경로를 지정해 셰이더 바이너리(.bin)를 먼저 생성합니다.

환경 변수 임시 등록: (DLL 위치를 알려줌)

PowerShell

$dxc_dir = "C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64"
$env:Path = "$dxc_dir;" + $env:Path
Donut 공통 셰이더 빌드:

PowerShell

& "D:\Studying\Streamline_Sample\_bin\ShaderMake.exe" --config "D:/Studying/Streamline_Sample/donut/shaders/DonutShaders.cfg" --out "D:/Studying/Streamline_Sample/_bin/shaders/donut/dxil" --platform DXIL --binaryBlob --outputExt .bin -I "D:/Studying/Streamline_Sample/donut/include" --compiler "$dxc_dir\dxc.exe" --shaderModel 6_5 --useAPI

Sample 전용 셰이더 빌드:

PowerShell

& "D:\Studying\Streamline_Sample\_bin\ShaderMake.exe" --config "D:/Studying/Streamline_Sample/src/shaders.cfg" --out "D:/Studying/Streamline_Sample/_bin/shaders/StreamlineSample/dxil" --platform DXIL --binaryBlob --outputExt .bin -I "D:/Studying/Streamline_Sample/donut/include" -D TARGET_D3D12 --compiler "$dxc_dir\dxc.exe" --shaderModel 6_5 --useAPI

3. 해결 단계 2: Visual Studio 빌드 예외 설정
수동으로 만든 셰이더를 사용하도록 하고, 에러 나는 프로젝트는 빌드에서 제외합니다.

구성 관리자 진입: 빌드(Build) -> 구성 관리자(Configuration Manager)

체크 해제: 아래 두 프로젝트의 빌드(Build) 체크박스 해제

donut_shaders

StreamlineSample_shaders

메인 빌드: StreamlineSample 프로젝트만 우클릭하여 빌드 수행.

4. 해결 단계 3: 실행 환경 구성 (DLL 배치)
실행 파일(.exe)이 있는 _bin 폴더에 필수 런타임 파일들을 모아줍니다.

SDK DLL 복사: Streamline SDK의 bin/x64/ 내 모든 파일 (sl.common.dll, sl.interposer.dll 등)을 _bin 폴더로 복사.

셰이더 확인: _bin/shaders/ 폴더 안에 수동으로 생성한 dxil 폴더와 .bin 파일들이 있는지 확인.
