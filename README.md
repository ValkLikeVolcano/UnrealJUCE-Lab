Third-Party Library Setup:

To use this project, you'll need to add the JUCE third-party library yourself. Follow these steps:

- Obtain JUCE: Download the latest JUCE framework from https://juce.com
- Create a Static Library: Use Projucer to create a static library for the JUCE modules you need
- Add to Project: Place the static library and the corresponding JUCE modules folder in a ThirdPartyFolder within your plugins's directory

Note: Ensure that the paths to the third-party library and modules are correctly configured in your project's build.cs.

To use this project with ASIO drivers, ensure ASIO drivers are installed and configured on your system. 
Then, in the Projucer, enable ASIO support in the "JUCE Audio Devices" module before building the project.
