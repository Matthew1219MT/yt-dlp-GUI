# yt-dlp-GUI
A GUI version for the open source program yt-dlp

# Build Instruction
1. Get your pc installed with Qt
2. Get your VS Code installed with Qt extension
3. Open this project in VS Code
4. Navigate to the `RUN AND DEBUG` tab on the left
5. Click the green triangle button to start debug and build the project
6. Open this project in file explorer
7. In the file explorer, open the build folder in this project, you should see an exe file inside
8. In the /build folder, create another folder named `exe`
9. Copy the yt-dlp-GUI.exe to this newly created `exe` folder
10. Open Qt terminal (Namely `QtVERSION_NAME (MinGW ANOTHER_VERSION_NAME 64-bit)`) 
11. In the terminal, change the directory to this new exe folder 
12. run `windeployqt.exe .` to package the exe to an independent version
13. Download / build `yt-dlp` exe from official github website
14. Put `yt-dlp` and `yt-dlp-GUI` at the same folder
15. Run `yt-dlp-GUI` and start downloading videos and audios !