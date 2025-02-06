#[cfg(test)]
mod echo_tests {

    use utils::{paths, runner};

    #[test]
    fn echo_test() {
        let maelstrom_jar = paths::maelstrom_dir().join("maelstrom.jar");
        let echo_path = paths::bazel_runfiles_dir()
            .join("echo-cpp")
            .join("echo-cpp");

        let o = runner::maelstrom_cmd(maelstrom_jar, echo_path, "echo")
            .output()
            .unwrap();

        if !o.status.success() {
            println!("stdout: {}", String::from_utf8(o.stdout).unwrap());
            println!("stderr: {}", String::from_utf8(o.stderr).unwrap());
        }
        assert!(o.status.success())
    }
}
