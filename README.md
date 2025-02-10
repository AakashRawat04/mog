# Mog - My Own Git Implementation

[![Progress Banner](https://backend.codecrafters.io/progress/git/f5a75a5c-fb7c-4664-b9c5-68ae072639e1)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

## Table of Contents
- [Introduction](#introduction)
- [Implementation Details](#implementation-details)
- [Git Internals](#git-internals)

## Introduction
Mog is my own implementation of Git, built from scratch in C++. 🚀

There are broadly two types of commands in git:
1. **Porcelain Commands**: High-level commands that users interact with directly (e.g., `git add`, `git commit`, `git push`)  
2. **Plumbing Commands**: Low-level commands that interact with the Git database (e.g., `git hash-object`, `git cat-file`, `git update-ref`)

Mog focuses on the plumbing commands and aims to provide a basic understanding of how Git works under the hood. It supports the following plumbing commands:
- `mog init`: Initialize a new Git repository
- `mog hash-object <file>`: Compute the hash of a file and store it in the Git database
- `mog cat-file <hash>`: Retrieve and display the contents of a Git object
- `mog write-tree`: Create a tree object from the current working directory
- `mog read-tree <tree-hash>`: Read the contents of a tree object into the working directory
- `mog commit-tree <tree-hash> [-p <parent-hash>]`: Create a commit object from a tree object


## Implementation Details
I wanted to understand how Git works under the hood, and I figured the best way to do that was to build my own implementation from the ground up. Along the way, I also deepened my knowledge of C++ and how to use it to develop a full-fledged application.

## Git Internals
Git operates as a **key-value store** based on three primary object types:

### Object Types Overview
1. **Blob Objects** - Store file contents
2. **Tree Objects** - Represent directories
3. **Commit Objects** - Capture repository snapshots

![types of objects](public/object%20types.png)

### Blob (Binary Large Object)
A **blob** stores the contents of a file. Its format is:

```
blob <content-length>\0<content>
```
- `<content-length>`: The length of the file content in bytes
- `<content>`: The actual file content

### Tree Object
A **tree** stores information about directories and the files they contain. Its format is:

```
tree <content-length>\0<entry>
```
Each entry in a tree object follows this structure:

```
<mode> <filename>\0<hash>
```
- `<mode>`: File mode (permissions)
- `<filename>`: Name of the file or directory
- `<hash>`: SHA-1 hash of the corresponding blob or tree object

![tree object](public/tree%20representation.png)

### Commit Object
A **commit** represents a snapshot of the repository at a specific point in time. Its format is:

```
commit <content-length>\0<commit-content>
```
The **commit content** includes:

```
tree <tree-hash>
parent <parent-hash>
author <author>
committer <committer>
<commit-message>
```
- `<tree-hash>`: Reference to the tree object representing the state of the repository
- `<parent-hash>`: Reference to the previous commit (optional for the first commit)
- `<author>`: The person who created the commit
- `<committer>`: The person who committed the changes
- `<commit-message>`: A message describing the commit
