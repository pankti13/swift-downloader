# SwiftDownloader 

Swift Downloader is a cross-platform, console-based application built using the Qt framework in C++, enabling multi-threaded HTTP/HTTPS file downloads with efficient use of network resources.

## How it Works 🔎

- Executes multi-threaded downloads by dividing the target file into chunks and assigning each chunk to a worker thread. Synchronization between threads is efficiently managed using semaphores.  
- Automatically handles URL redirects to ensure the correct file is downloaded to the user-specified location.  

## Setup 🔧

### For Windows

1. Download and Install the following tools
   ```bash
   qt-opensource-windows-x86-5.9.8.exe, from https://qthub.com/static/doc/qt5/qtdoc/windows.html
   CMake (Version >= 3.1.0) from https://cmake.org/download/
   Win32/Win64 OpenSSL from https://slproweb.com/products/Win32OpenSSL.html

2. Build the project
   ```bash
   cmake-gui
   
- Follow instructions similar to that of the linux environment
   
### For Ubuntu 14.04 and above

1. Install the necessary tools and libraries by executing the following command in the terminal
   ```bash
   sudo apt install qtcreator qtbase5-dev libcurl4-openssl-dev build-essential cmake
   
2. Clone the Swift Downloader repository from GitHub
   ```bash
   git clone https://github.com/pankti13/swift-downloader.git
   
3. Navigate to the project directory and configure the build environment
   ```bash
   cd swift-downloader/
   mkdir build
   cd build
   cmake ..
   make

4. Execute the Swift Downloader: `./Swift-Downloader`

## Screenshots 📷

1. Input Form page ![InputForm](https://github.com/user-attachments/assets/b9818349-e0e3-4e66-b6e7-7b2f1638af3e)
   
2. Show Progress page ![ProgressPage](https://github.com/user-attachments/assets/d98b6809-affc-4e9f-bc7f-598ad99c6d9b)

## Problem it Solves 🚀

In today’s world, downloading large files can often be a time-consuming and inefficient process. Without proper optimization, significant network bandwidth often goes unused, leading to slower download speeds. 
Swift Downloader optimizes network resource utilization, enabling faster and more efficient downloads of large files while reducing download time and minimizing network strain.

## Future Improvements 💬

- Display download history for the user.
- Enhance the app's UI based on user feedback.
- Show current network stats and the total data required for file download in the UI.
- Allow users to specify the number of threads for downloading.

## Contributions 🤝

Contributions to this project are always welcome! If you have any issues or suggestions, open an issue, submit a pull request, or email me at panktisalvi1305@gmail.com.
