use std::env;
use std::path::PathBuf;

pub fn cargo_bin_dir() -> PathBuf {
    env::var_os("CARGO_BIN_PATH")
        .map(PathBuf::from)
        .or_else(|| {
            env::current_exe().ok().map(|mut path| {
                path.pop();
                if path.ends_with("deps") {
                    path.pop();
                }
                path
            })
        })
        .unwrap_or_else(|| panic!("CARGO_BIN_PATH wasn't set. Cannot continue running test"))
}

pub fn bazel_runfiles_dir() -> PathBuf {
    PathBuf::from(env::var_os("RUNFILES_DIR").unwrap()).join("_main")
}

pub fn maelstrom_dir() -> PathBuf {
    bazel_runfiles_dir()
        .join("tests")
        .join("maelstrom")
        .join("lib")
}
