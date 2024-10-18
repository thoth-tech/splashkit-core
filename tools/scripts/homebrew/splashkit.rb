class Splashkit < Formula
  desc "SplashKit software toolkit"
  homepage "https://splashkit.io"
  url "https://github.com/The-0x/skm/archive/refs/tags/1.0.0.zip"
  version "1.0.0"
  sha256 "e8c80832ad2b007357d559598a90bdc0ffc3fee08e276cf85cf2a60e8efccc42"
  
  def install
    ohai "Installing SplashKit software toolkit"
  
    
    # Copy the Splashkit lib to /usr/local/lib
    lib.install "lib/macos/libSplashKit.dylib"

    # Copy all headers to include/splashkit
    (include/"splashkit").install Dir["clang++/include/*"]

    ##Using the `.install` command removes the files from the .zip so we cannot get the splashkit.h the same way
    ##If we were able to create a symbolic link (line 36) this wont be a  problem
      #Copy splashkit.h to /usr/local/include
      #include.install "clang++/include/splashkit.h"
    

    ##Do not have the permission to do this, Homebrew  doesn't allow  for any editing of the HOME path##
      #Create symbolic links to /usr/local/lib and /usr/local/include

      #ohai "Creating symbolic links to /usr/local"
      #libpath = "/usr/local/lib/libSplashKit.dylib"
      #headerpath = "/usr/local/include/splashkit.h"
      #include_folder_path = "/usr/local/include/splashkit"
      
      # Create symbolic link for the library
      #system "ln", "-sf", lib/"libSplashKit.dylib", libpath
      
      # Create symbolic link for splashkit.h
      #system "ln", "-sf", include/"splashkit/splashkit.h", headerpath

      # Create symbolic links for all headers in splashkit include folder
      #system "ln", "-sf", include/"splashkit", include_folder_path

    ohai "SplashKit installation completed!"
  end

  test do
    (testpath/"test.cpp").write <<~EOS
    #include "splashkit.h"

    int main()
    {
      write_line("SplashKit is installed correctly!");
      return 0;
    }
    EOS
    system "clang++", "test.cpp", "-LSplashKit" , "-o", "test"
    system "./test"
  end
end
