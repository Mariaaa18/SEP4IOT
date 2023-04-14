![IoT Base logo](proj_pages/images/IoTBase.png)
# IoT demo/base project
This project shows how a development environment for the IotT target (with the ATMega2560 processor) can be setup to run inside a development container. You can build directly uploadable firmware inside the container.

Click here to get to the [**project pages w. downloads and test results**](https://dafessor.github.io/iot-base/).

# How to use this project/setup

## Prerequisites
Make sure that you have these apps installed:
* Docker Desktop
* WSL2
* Visual Studio Code
  
 You will also need (at least) these extension installed in VSCode before you start:
   * *Dev Containers* by Microsoft
   * *Remote Explorer* by Microsoft
   * *WSL* by Microsoft
   * *GitHub Repositories* by GitHub

Make sure that Docker Desktop is running.

## Step-by-step
1. Start VSCode and press the *Remote Explorer* icon on the left.

2. Select *Dev Containers* at the drop-down menu at the top ogf the explorer(if it is not already chosen by default).

3. Place the cursor over the *DEV VOLUMES* section at the bottom of the explorer and let it hover over the leftmost small icon that gets shown.

4. Choose *Clone Repository in Container Volume*

5. Select the GitHub project *DaFessor/iot-base* and choose the *main* branch. **Note:** If you clone the *DaFessor/iot-base* project, you should obviously choose your own cloned copy of the repo.

6. `>>>` VSCode will now do its stuff, just wait for it to complete. One of the handy features of VSCode is that it at this point automatically installs a list of necessary/relevant extensions for use inside the container. The list is configured in `.devcontainer/devcontainer.json`

7. Now we're ready to go on. If you at this point get prompted to choose a CMakeLists.txt file or a toolchain/compiler, just keep ignoring it (press ESC). We'll do the final cmake setup in a later step.

8. Once VSCode is done and you can see the project explorer to the left, click the *.vscode* folder and open the file called `iot-demo.code-workspace`.

9.  A button labelled *Open Workspace* opens at lower right corner - click it.

10. `>>>` VSCode now changes into what's known as a multi-root workspace setup. This means we can work with multiple, independent sub-projects open under the umbrella of a single top-level root folder. In our actual setup there are 3 sub-projects:

    * **Project Root** - this is the top folder. It's not really a buildable project, but it's here to provide easy access to the important files in the top folder.
    * **Target** - this is where all the stuff that's built for the target IoT device resides. Inside here there's a CMake managed cross-compile setup that allows us to build code that will run on the Atmega2560. The resulting binaries can obviously not run on our laptops, but once they are built automatically on GitHub, they can be downloaded from there. You can also fiddle about with the firmware files here once they are built by the cross-compiler toolchain.
    * **Test** - this is a project setup with Google test so that files (presumably fetched from the Target project somewhere - don't copy files from target, just create symbolic links to the files that should be tested) can be built and tested locally at our own laptops. All defined tests will also be automatically built on GitHub.

## Final cmake config/initialization
11. We're now in multi-root mode and there's a CMake icon on the left. Click it, and here we can now see the projects as cmake sees them.
    
12. Click on the small icon on the top with the fly-by text *Configure All Projects*. CMake now configures our 2 projects (test & target).
    
13. Expand the two interesting projects
    *  `/workspaces/iot-base/test`, and
    *  `/workspaces/iot-base/target`

14. Each of these two (target & test) have an entry with a small hammer icon. If we click on one these our project (target firmware files or our tests)  gets built.

## Done
Now we're done, our repository has been checked out and we can build everything inside a development container created and managed by VSCode.

In addition to building code via the "cmake view", we can use the small buttons/icons at the bottom of VSCode. Here we can:
* Choose our active (sub) project that we want to build (test or target)
* Choose which configuration to build (release, debug etc.)
* Choose which toolchain to use for builds (GCC_12.2.0-avr or "normal" gcc=GCC_12.2.1-x86_64-alpine-linux-musl) - but tests should be built with normal gcc and target with avr.
* Choose which build target to actually build, but just use "all" that'll work.
* Run our code (that only works for tests) by clicking on the "play-button". That's one way to all our tests.
* We can also run our tests by clicking on the "tickmark"-icon.

You can at all times press F7, that will build your currently selected project.

## Doing a CMake (re-) configure
The CMake configure thing is normally only done once, but if you change anything in a CMakeLists.txt file, you will need to (re-) configure things. That's done like this:
* Choose the CMake view/explorer
* Click the kebab menu (the three dots `...`) in the upper right corner
* Choose *Clean Reconfigure All Projects*

..... aaaaand you're done.

## Debugging
You can start a debug session on the test code, but first you have to open a terminal and type

`apk add gdb`

to install the debugger. The debugger is not part of the default devcontainer image because installing it takes up a lot of space (~500Mb). But it only takes a few seconds to install it it you need it.

## Starting and stopping work
Cloning your repository to a container volume may seem odd at first. It means that everything is inside the volume, and you can't browse or fiddle with the files in a local Window repository. But then why would you, you can always (re-) start the development container which automatically mounts the volume. By using VSCode as your sole window into the project we gain some advantages, and you can do all you need with the project inside VSCode anyway.

Just close the project using *File/Close Remote/Connection* when you want to put the project aside. When you want to work on the project again, just use the VSCode *File/Open Recent* menu to re-open the development container (it shows up as something like `/workspaces/iot-base/.vscode/iot-demo (Workspace)[Dev Container:C++]`), and carry on from where you left.

If your project has disappeared from the *File/Open Recent* menu, you can re-open your project like this (provided that you haven't deleted your development container):
* Start VSCode and click on the Remote Explorer icon.
* List development containers (make sure the drop-down in the upper right corner shows *Dev Containers*)
* Find your development container
* Select one of the workspaces (folders) listed under the container and click on the small folder icon. That will start the development container and make it open the selected workspace.

## If/when shit happens
As long as you _**don't unintentionally delete the container volume**_ you can always retrieve any work that hasn't been committed and pushed to GitHub.

You can use Docker Desktop to export the contents of volume as a file in a local Windows folder. That will produce a .tar.zst formatted file that you can extract using the zstd command in a WSL/Linux shell.