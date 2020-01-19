# BackUp
The incremental backup program with restoring on previous versions.

Controlled by switches:

    --init      "source path" "destination path"                Create a repository for backup and make first full backup.

    --delete    "repository path/id"                            Delete the repository.

    --show                                                      Print all initializated repositories.

    --open      "repository path/id"                            Open the repository and print overview of all backups.

        --diff      "id first backup" "id second backup"        Print difference between two backups.

        --backup                                                Check if backup is current in case of not will make a backup.

        --time                                                  Set time for automatic backup.