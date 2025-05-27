# Building Lowstepper locally

Within `.vscode/c_cpp_properties.json`:

- set the contents of `includePath` to be the relative path of your Rack-SDK folder
- change the field `compilerPath` to your g++ path

## Setup VSCode Tasks

### Windows

Within `.vscode/tasks.json` , modify all instances of the following JSON fields:

```
"env": {
    "PATH": <insert msys64/user/bin and msys64/mingw64/bin absolute paths, seperated by semicolon>
    "RACK_DIR" <absolute directory of your Rack-SDK folder>
}

// Example
"env":{
    "PATH": "C:/msys64/usr/bin/;C:/msys64/mingw64/bin",
    "RACK_DIR": "C:/Users/You/Documents/Rack-SDK"
}
```