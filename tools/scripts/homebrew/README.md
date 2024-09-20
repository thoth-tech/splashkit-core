
# Homebrew Package (WIP)

This directory contains the current iteration of the `splashkit.rb` formula. The full functionality has not been implemented 
and further trouleshooting and testing is required, this documentation shows the developement of what has been created so far. 

Note: This document refers to a `splashkit-1.0.0.zip` file which is obtained [here](https://github.com/The-0x/skm/releases/tag/1.0.0)
(file is too big to upload to GitHub unfrotunately).

## `skm-1.0.0.zip`

This compressed folder contains the files that need to be installed by homebrew, it needs to be available to download via a URL. 
During my time developing this, a file uploaded to GitHub that was a compressed version obtained the following error when trying to 
use it, `ditto: Couldn't read PKZip signature`. This was resolved by making use of the `Release` feature on GitHub. This feature 
creates a readily available source code, containing all elements of the repository, so for this task the `skm` repository was used 
to craete a `skm-1.0.0.zip/.tar.gz` file.

## Building the Formula (`splashkit.rb`)

After creating the `skm-1.0.0.zip` file, the download link is needed in order to start creating the formula that homebrew will use. 

Run the command:
`brew create <skm-1.0.0 download URL>`

This command will open a template of a Homebrew formula. The splashkit.rb file is the devloped forumla that contains the instructions
 for the homebrew installer on what to do with the provided file. More infromation can be found in the Homebrew 
 [Formula Cookbook](https://docs.brew.sh/Formula-Cookbook).

The `desc`, `homepage` & `version` header all provide their respective infromation, the `url` header tells the package where to download 
the file from, this is where the download link for the `skm-1.0.0.zip` file should be (The URL in the current `splashkit.rb` is linked to 
a fork of the current skm of my account) & the `sha256` provides the checksum for the package.

### `def install`

During the `install` section of the package we attempt to move the files located in `skm-1.0.0.zip` into the intended directory. We cannot 
directly install into `usr/local/` so instead predefined variables are used (`lib` & `include`) that are linked to the Homebrew Cellar 
directory. As mentioned before Homewbrew doesn't allow for any manipulation of the `/usr/local/` directory so symbolic links were attempted 
to circumvent this. However, this is still seen as manipulation of the `/usr/local/` directory so we were not permitted even with the use 
of `sudo` in these commands.

### `test do`

The test section of the `splashkit.rb` file createas, compiles and runs a simple test program that uses Splashkit to see if the installation 
was a success. 

## Testing issues & moving forward

To use the formula created, run:

`brew install --build-from-source splashkit`

Note: if the above command doesn't work try using 'skm' instead of 'splashkit'.

This command will then download the needed files, run the install function and initiate the test to ensure that the package was installed 
correctly. However, with the current formula we are unable to move forward from this step, as our formula tries to alter `/user/local/` which 
Homebrew do not want us to do. This is the current roadblock for this iteration of the package. Splashkits functionality stems from the 
`HOME` path environment containing the files that are needed.

Moving forward, the focus should be on what other methods can be used to ensure that the installed files can be located by the system, external 
commands defeat the purpose of creating a package, as the package should be the only source of installation. 

When functional, to get the formula to be a part of the homebrew installer a commit and push to the official `homebrew/core` repository will 
create a pull request that will be reviewed by homebrew.