#include <iostream>
#include <string>

#include <openvr.h>

int main()
{
    vr::EVRInitError error = vr::VRInitError_None;

    vr::IVRSystem* vr_system = vr::VR_Init(&error, vr::VRApplication_Background);

    if (error != vr::VRInitError_None)
    {
        std::cerr << "Failed to initialize OpenVR: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;

        return 1;
    }

    std::cout << "OpenVR initialized successfully.\n";

    for (uint32_t i=0; i < vr::k_unMaxTrackedDeviceCount; ++i)
    {

        if (!vr_system->IsTrackedDeviceConnected(i))
        {
            continue;
        }

        vr::ETrackedDeviceClass device_class = vr_system->GetTrackedDeviceClass(i);

        std::string device_type;

        switch (device_class)
        {
            case vr::TrackedDeviceClass_HMD:
                device_type = "HMD";
                break;
            
            case vr::TrackedDeviceClass_Controller:
                device_type = "Controller";
                break;
            
            case vr::TrackedDeviceClass_GenericTracker:
                device_type = "Tracker";
                break;
            
            case vr::TrackedDeviceClass_TrackingReference:
                device_type = "Base Station";
                break;

            default:
                device_type = "Other";
                break;
        }

        char serial[128] = {};

        vr_system->GetStringTrackedDeviceProperty(i,
                                                vr::Prop_SerialNumber_String,
                                                serial,
                                                sizeof(serial));

        char model[128] = {};

        vr_system->GetStringTrackedDeviceProperty(i,
                                                vr::Prop_ModelNumber_String,
                                                model,
                                                sizeof(model));
                                                
        std::cout   << "Device " << i
                    << " | Type: " << device_type
                    << " | Model: " << model
                    << " | Serial " << serial
                    << std::endl;

    }

    vr::VR_Shutdown();

    return 0;
}