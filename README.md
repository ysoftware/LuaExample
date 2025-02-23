### Lua from C.

- Initialize Lua (with a custom allocator).
- Set up the stack and call a Lua function with arguments from C.
- Provide a C procedure to be called from Lua.
- Print out the return value of the function.

```console
$ make && ./main
clang main.c -Ilua-5.4.7/src lua-5.4.7/src/liblua.a -lm -o main
Adding:	25	24	20
Result of add is 69.
```
