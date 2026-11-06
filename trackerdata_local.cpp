#include <iostream>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <thread>
#include <cmath>

#include <openvr.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string>

struct Quaternion
{
	double w;
	double x;
	double y;
	double z;
};

Quaternion rotationMatrixToQuaternion(const vr::HmdMatrix34_t& matrix)
{
	const double r00 = matrix.m[0][0];
	const double r01 = matrix.m[0][1];
	const double r02 = matrix.m[0][2];

	const double r10 = matrix.m[1][0];
	const double r11 = matrix.m[1][1];
	const double r12 = matrix.m[1][2];

	const double r20 = matrix.m[2][0];
	const double r21 = matrix.m[2][1];
	const double r22 = matrix.m[2][2];

	Quaternion q;

	const double trace = r00 + r11 + r22;

	if (trace > 0.0)
	{
		const double s = 0.5 / std::sqrt(trace + 1);

		q.w = 0.25 / s;
		q.x = (r21 - r12) * s;
		q.y = (r02 - r20) * s;
		q.z = (r10 - r01) * s;
	}
	else if (r00 > r11 && r00 > r22)
	{
		const double s = 2.0 * std::sqrt(1.0 + r00 - r11 - r22);

		q.w = (r21 - r12) / s;
		q.x = 0.25 * s;
		q.y = (r01 + r10) / s;
		q.z = (r02 + r20) / s;
	}
	else if (r11 > r22)
	{
		const double s = 2.0 * std::sqrt(1.0 + r11 - r00 - r22);

		q.w = (r02 - r20) / s;
		q.x = (r01 + r10) / s;
		q.y = 0.25 * s;
		q.z = (r12 + r21) / s;
	}
	else
	{
		const double s = 2.0 * std::sqrt(1.0 + r22 - r00 - r11);

		q.w = (r10 - r01) / s;
		q.x = (r02 + r20) / s;
		q.y = (r12 + r21) / s;
		q.z = 0.25 * s;
	}

	return q;

}

int main()
{

	vr::EVRInitError error = vr::VRInitError_None;

	vr::IVRSystem* vr_system = vr::VR_Init(&error, vr::VRApplication_Background);

	if (error != vr::VRInitError_None)
	{
		std::cerr << "Failed to initialize OpenVR: "
			<< vr::VR_GetVRInitErrorAsEnglishDescription(error)
			<< std::endl;

		return 1;
	}

	std::cout << "OpenVR initialized successfully." << std::endl;

	//Find the first generic tracker
	vr::TrackedDeviceIndex_t tracker_index = vr::k_unTrackedDeviceIndexInvalid;

	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
	{
		if (!vr_system->IsTrackedDeviceConnected(i))
		{
			continue;
		}

		vr::ETrackedDeviceClass device_class = vr_system->GetTrackedDeviceClass(i);

		if (device_class == vr::TrackedDeviceClass_GenericTracker)
		{
			tracker_index = i;
			break;
		}
	}

	if (tracker_index == vr::k_unTrackedDeviceIndexInvalid)
	{
		std::cerr << "No Vive Tracker found." << std::endl;

		vr::VR_Shutdown();

		return 1;
	}

	// get tracker serial number
	char serial[128] = {};

	vr_system->GetStringTrackedDeviceProperty(tracker_index,
		vr::Prop_SerialNumber_String,
		serial,
		sizeof(serial));

	char model[128] = {};

	vr_system->GetStringTrackedDeviceProperty(tracker_index,
		vr::Prop_ModelNumber_String,
		model,
		sizeof(model));

	std::cout << "Tracker found!" << std::endl;
	std::cout << "Index : " << tracker_index << std::endl;
	std::cout << "Model : " << model << std::endl;
	std::cout << "Serial : " << serial << std::endl;

	std::cout
		<< "sequence, "
		<< "Timestamp, "
		<< "x, y, z, "
		<< "qx, qy, qz, qw, "
		<< "vx, vy, vz, "
		<< "wx, wy, wz"
		<< std::endl;

	uint64_t sequence = 0;
	const auto start_time = std::chrono::steady_clock::now();

	while (true)
	{
		vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];

		vr_system->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding,
			0.0f,
			poses,
			vr::k_unMaxTrackedDeviceCount);

		const vr::TrackedDevicePose_t& pose = poses[tracker_index];

		if (pose.bPoseIsValid)
		{
			//Timestamp
			const auto current_time = std::chrono::steady_clock::now();

			const double timestamp = std::chrono::duration<double>(current_time - start_time).count();

			//Linear velocity
			float vx = pose.vVelocity.v[0];
			float vy = pose.vVelocity.v[1];
			float vz = pose.vVelocity.v[2];

			//angular
			float wx = pose.vAngularVelocity.v[0];
			float wy = pose.vAngularVelocity.v[1];
			float wz = pose.vAngularVelocity.v[2];

			//position
			float x = pose.mDeviceToAbsoluteTracking.m[0][3];
			float y = pose.mDeviceToAbsoluteTracking.m[1][3];
			float z = pose.mDeviceToAbsoluteTracking.m[2][3];

			Quaternion orientation = rotationMatrixToQuaternion(pose.mDeviceToAbsoluteTracking);

			//output

			std::cout

				<< sequence << ","
				<< std::fixed
				<< std::setprecision(6)
				<< timestamp << ","
				<< x << ","
				<< y << ","
				<< z << ","
				<< orientation.x << ","
				<< orientation.y << ","
				<< orientation.z << ","
				<< orientation.w << ","
				<< vx << ","
				<< vy << ","
				<< vz << ","
				<< wx << ","
				<< wy << ","
				<< wz
				<< std::endl;


			sequence++;
		}
		else
		{
			std::cout << "Tracker pose Invalid" << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10)); //100 Hz
	}

	vr::VR_Shutdown();

	return 0;
}