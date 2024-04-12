import os

def append_file_contents_to_repo(start_dir, repo_file_path):
    """
    Recursively append the contents of all files in start_dir and its subdirectories
    to a single file specified by repo_file_path.
    """
    for root, dirs, files in os.walk(start_dir):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    contents = f.read()
                with open(repo_file_path, 'a', encoding='utf-8') as repo:
                    repo.write(f"<START OF {file_path}>\n")
                    repo.write(contents)
                    repo.write(f"\n<END OF {file_path}>\n")
            except Exception as e:
                print(f"Failed to process {file_path}: {e}")

if __name__ == "__main__":
    repo_file = "repo.txt"
    start_directory = "."  # Current directory
    append_file_contents_to_repo(start_directory, repo_file)
