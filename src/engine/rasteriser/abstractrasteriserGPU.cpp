#include "abstractrasteriserGPU.h"

void AbstractRasteriserGPU::initialize_opencl() {
  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);

  if (all_platforms.empty()) {
      std::cout << "No platform found" << std::endl;
      exit(1);
  }

  unsigned platform_index = 0;
  if (all_platforms.size() > 2)
        platform_index = 1;

  cl::Platform default_platform = all_platforms[platform_index];		//change to 1 for cuda
  std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

  std::vector<cl::Device> all_devices;
  default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
  if (all_devices.empty()) {
    std::cout << "No device found" << std::endl;
    exit(1);
  }

  device = all_devices[0];
  std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;

  context = cl::Context ({device});
  queue = cl::CommandQueue (context, device);
}

void AbstractRasteriserGPU::load_kernel(std::string path,
                                        std::string kernel_name) {
  std::ifstream file;
  file.open (path);

  std::cout << "Loading kernel" << std::endl;

  if (!file.is_open()) {
    std::cerr << "Error loading kernel " << path << std::endl;
    return;
  } else {
    std::cout << "Kernel succesfully loaded" << std::endl;
  }

  std::string kernel_str ((std::istreambuf_iterator<char>(file) ),
                          (std::istreambuf_iterator<char>()));

  sources.push_back({kernel_str.c_str(), kernel_str.length()});

  std::cout << "Compiling kernel" << std::endl;
  program = cl::Program (context, sources);
  if (program.build({device}) != CL_SUCCESS) {
    std::cerr << "Error compiling kernel: \n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
    return;
  }

  std::cout << "Kernel succesfully compiled" << std::endl;

  kernel = cl::Kernel (program, kernel_name.c_str());
  ready = true;
}
