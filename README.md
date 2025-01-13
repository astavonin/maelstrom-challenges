# maelstrom-challenges

## How to Run

To run tests, you should run the `build` command first, and only after that, run the tests. While the preferred approach
would be only called `test`, but there is no straightforward way to tell `Cargo` to build binary before test execution (
see comments in [echo/Cargo.toml](echo/Cargo.toml) and [Cargo.toml](Cargo.toml)).

```bash
cargo build
cargo test
```

To generate `compile_commands.json` use
```bash
bazel run @hedron_compile_commands//:refresh_all
```
