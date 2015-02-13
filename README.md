Hatch
=====

Hatch is a toy intepreter for evaluating prefix integer math expressions.

```
> - + * 2 8 / 15 5 4
15
```

More formally, Hatch is a Recursive-Descent LL(1) Parser hand written in
C.

There are no variables, files, or anything fancy; only a
[REPL](https://en.wikipedia.org/wiki/Read-eval-print_loop).

Available operators are:

- `*`, `/`
- `+`, `-`

These respectively represent (semantically) multiplication, division,
addition, and subtraction.

To exit the REPL use `Ctrl-D` or `Ctrl-C`.

[License](LICENSE) GPLv3
