#[cfg(test)]
mod echo_tests {

    use utils::{paths, runner};

    static WORKFLOW: &str = "broadcast";

    #[test]
    fn broadcast_test() {
        let maelstrom_jar = paths::maelstrom_dir().join("maelstrom.jar");
        let echo_path = paths::bazel_runfiles_dir().join(WORKFLOW).join(WORKFLOW);

        let o = runner::maelstrom_cmd(maelstrom_jar, echo_path, WORKFLOW)
            .output()
            .unwrap();

        if !o.status.success() {
            println!("stdout: {}", String::from_utf8(o.stdout).unwrap());
            println!("stderr: {}", String::from_utf8(o.stderr).unwrap());
        }
        assert!(o.status.success())
    }
}
