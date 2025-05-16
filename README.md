# GGenie build and run

## Install Python
```
pyenv install 3
```
or other way

## Install Conan 
[Conan install guide](https://docs.conan.io/2/installation.html)
<details>
  
<summary>pyenv-virtualenv install</summary>

<br/>Inside project directory
```
pyenv virtualenv 3.13.3 ggenie_env
pyenv local ggenie_env
pip install conan
```
</details>
<details>

<summary>Global install</summary>

<br/>

```
pip install conan
```
</details>

## Build
```
conan install . -s build_type=Debug
cmake --preset=conan-debug
cmake --build --preset conan-debug 
```

## Run
```
build/engine3d
```
