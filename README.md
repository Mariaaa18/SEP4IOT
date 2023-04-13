# IoT demo/base project
This project shows how a development environment can be setup to run inside a development container.

Click here to get to the [project pages w. downloads and test results](https://dafessor.github.io/iot-base/).


## How to use this project/setup

1. Make sure that you have Docker Deskto, WSL2 and Visual Studio Code installed.
2. You will also need (at least) these extension installed in VSCode:
   * *Dev Containers* by Microsoft
   * *Remote Explorer* by Microsoft
   * *WSL* by Microsoft
   * *GitHub Repositories* by GitHub
3. Make sure that Docker Desktop is running.
4. Start VSCode and press the *Remote Explorer* icon on the left.
5. Select *Dev Containers* at the drop-dwon menu at the top (if it is not already chosen by default).
6. Place the cursor over the *DEV VOLUMES* section at the bottom and let it hover over the leftmost small icon that gets shown.
7. Choose *Clone Repository in Container Volume*
8. Select the GitHub project *DaFessor/iot-base* and choose the *main* branch.
9. ------ VSCode will now do its stuff, just wait for it to complete. One of the handy features of VSCode is that it also automaticall installs a list of necessary/relevant extensions for use inside the container. The list is configured in `.devcontainer/devcontainer.json`
10. Once it's done and you can see the project explorer to the left, click the *.vscode* folder and open the file called `iot-demo.code-workspace`.
11. A button labelled *Open Workspace* opens at lower right corner - click it.
12. ------ VSCode now changes into what's known as multi-root workspace setup. This means we can work with multiple, independent sub-projects open under the umbrella of a single top-level root folder. In this setup there are 3 sub-projects:
    * **Project Root** - this is the top folder. It's not really a buildable project, but it's here to provide easy access to the important files in the top folder
    * **Target** - this is where all the stuff that's built for the target IoT device resides. Inside here there's a CMake managed cross-compile setup that allows us to build code that will run on the Atmega2560. The resulting binaries can obviously not run on our laptops, but once they are built automatically on GitHub, they can be downloaded from there. You can also fiddle about with the firmware files here once they are built by the cross-compiler toolchain.
    * **Test** - this is a project setup with Google test so that files (presumably fetched from the Target project somewhere - don't copy files from target, just create symbolic links to the files that should be tested) can be built and tested locally at our own laptops. All defined tests will also be automatically built on GitHub.
  13. The CMake extension has been automatically installed, and we can now click on the icon to left to build our code. Code for test and target can be built independently as needed.
