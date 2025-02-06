use std::path::PathBuf;
use std::process;

pub fn maelstrom_cmd(
    maelstrom_path: PathBuf,
    app_path: PathBuf,
    test_name: &str,
) -> process::Command {
    let mut cmd = process::Command::new("java");
    cmd.args([
        "-Djava.awt.headless=true",
        "-jar",
        maelstrom_path.to_str().unwrap(),
    ])
    .args(["test", "-w", test_name, "--bin", app_path.to_str().unwrap()])
    .args(["--time-limit", "10"]);
    cmd
}
