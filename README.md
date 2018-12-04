broadcasts a command to all running vim instances. this allows you to do
stuff like gracefully closing all open vim instances in one command,
even if you somehow lost access to the terminal it was running on

![](https://media.giphy.com/media/65zUG4M4DSVo624zJr/giphy.gif)

this is only for linux and unix-like OSes that have TIOCSTI

I was gonna add more advanced features to this like matching specific
vim instances but I lost interest because TIOCSTI requires root which makes
it feel pretty ugly

# install the binaries

download the binaries from [releases](https://github.com/Francesco149/vimpls/releases)

```sh
tar xf vimpls_x86_64.tar.gz
sudo install vimpls/vimpls /usr/local/bin
```

note that this only matches instances that were started with ```vim```
with no leading path to the binary

# usage
```sh
printf "\033:wqa\!\n" | sudo vimpls
```

# build from source

you need at least gcc installed

```
git clone https://github.com/Francesco149/vimpls
cd vimpls
./build
sudo install vimpls/vimpls /usr/local/bin
```

dietlibc can be used to get a smaller binary by pointing the environment
variable ```DIETBIN``` to the diet binary
