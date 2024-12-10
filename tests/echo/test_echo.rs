#[cfg(test)]
mod echo_tests {
    use utils::paths;
    use utils::runner;

    #[test]
    fn echo_test() {
        let maelstrom_jar = paths::maelstrom_dir().join("maelstrom.jar");
        let echo_path = paths::cargo_bin_dir().join("echo");

        let o = runner::maelstrom_cmd(maelstrom_jar, echo_path)
            .output()
            .unwrap();

        if !o.status.success() {
            println!("stdout: {}", String::from_utf8(o.stdout).unwrap());
        }
        assert!(o.status.success())
    }
}
