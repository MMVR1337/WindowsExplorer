### WindowsExplorer Documentation

#### Overview
The **WindowsExplorer** project is a recreation of the Windows 10 File Explorer using the ImGui library (v1.90.9). This project aims to provide a custom and extendable file management system with a familiar interface for users.

#### Getting Started
To get started with the **WindowsExplorer** project, you need to clone the repository and set up the development environment.

##### Prerequisites
- C++ Compiler (supporting C++17 or later)
- CMake (version 3.10 or later)
- Git

##### Clone the Repository
```bash
git clone https://github.com/MMVR1337/WindowsExplorer.git
cd WindowsExplorer
```

#### Installation
Follow these steps to build and run the project:

1. **Generate Build Files**
    ```bash
    cmake -S . -B build
    ```

2. **Build the Project**
    ```bash
    cmake --build build
    ```

3. **Run the Executable**
    ```bash
    ./build/WindowsExplorer
    ```

#### Usage
Upon running the executable, the custom file explorer interface will launch. You can navigate through directories, open files, and perform basic file operations such as copy, paste, and delete.

##### Key Bindings
- **Arrow Keys**: Navigate through items
- **Enter**: Open selected item
- **Delete**: Delete selected item
- **Ctrl+C**: Copy selected item
- **Ctrl+V**: Paste item

#### Features
- **File Navigation**: Navigate through directories and view file contents.
- **File Operations**: Perform basic file operations like copy, paste, and delete.
- **Customizable UI**: Interface built with ImGui, allowing for easy customization.

#### Contribution
Contributions are welcome! Please follow these steps to contribute:

1. **Fork the Repository**
2. **Create a Feature Branch**
    ```bash
    git checkout -b feature/YourFeature
    ```
3. **Commit Your Changes**
    ```bash
    git commit -m "Add some feature"
    ```
4. **Push to the Branch**
    ```bash
    git push origin feature/YourFeature
    ```
5. **Open a Pull Request**

#### License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

#### Contact
For any questions or suggestions, feel free to open an issue or contact the repository owner at [MMVR1337](https://github.com/MMVR1337).

---
