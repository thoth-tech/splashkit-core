from splashkit import *

def test_window():
    # Creates a window using SplashKit and shows it for 5 seconds
    window = open_window("Test Window", 800, 600)
    delay(5000)  # Wait for 5 seconds
    close_window(window)

if __name__ == "__main__":
    try:
        print("Testing SplashKit Window...")
        test_window()
        print("Test completed successfully!")
    except Exception as e:
        print(f"Test failed with error: {e}")
