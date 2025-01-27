# maelstrom-challenges

The repository contains implementations of [Maelstrom challenges](https://github.com/jepsen-io/maelstrom) in various programming languages (currently Rust and C++), using Bazel as the build system. It serves as my playground for experimenting with different build systems such as Cargo, CMake, Bazel, and potentially others in the future.

You may find these related articles insightful:

- [Returning to Rust: A Journey Through Tooling and Performance](https://sysdev.me/returning-to-rust-a-journey-through-tooling-performance/)
- [Bazel and Rust: A Perfect Match for Scalable Development](https://sysdev.me/bazel-and-rust/)
- [Managing Multi-Language Projects with Bazel](https://sysdev.me/multi-language-projects-with-bazel/)
- [How to Compile C++ in 2025: Bazel or CMake?](https://sysdev.me/how-to-compile-cpp-in-2025/)

## How to Run

Since the repository is Bazel-based, you can follow the typical Bazel workflow:

```bash
bazel build //...
bazel test //...
```

To generate a `compile_commands.json` file for better editor support, use:

```bash
bazel run @hedron_compile_commands//:refresh_all
```

### Running Rust Code with Cargo

If you'd like to run the Rust-specific part of the codebase using Cargo, follow these steps:

1. **Build the project:** Run the `cargo build` command to compile the binaries. 
2. **Run tests:** Execute `cargo test` after the build is complete.

While the preferred approach is to use the `cargo test` command directly, certain dependencies or configurations might require a prior build step. See comments in [echo/Cargo.toml](echo/Cargo.toml) and [Cargo.toml](Cargo.toml) for additional details.

```bash
cargo build
cargo test
```