use std::path::PathBuf;
use std::process;

pub fn maelstrom_cmd(maelstrom_path: PathBuf, echo_path: PathBuf) -> process::Command {
    let mut cmd = process::Command::new("java");
    cmd.args([
        "-Djava.awt.headless=true",
        "-jar",
        maelstrom_path.to_str().unwrap(),
    ])
    .args(["test", "-w", "echo", "--bin", echo_path.to_str().unwrap()])
    .args(["--time-limit", "10"]);
    cmd
}
