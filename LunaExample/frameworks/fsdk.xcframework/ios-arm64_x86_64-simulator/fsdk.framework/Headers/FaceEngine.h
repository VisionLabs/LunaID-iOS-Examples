/**
 * @file     	FaceEngine.h
 * @brief    	Face detection, analyzing and recognition SDK interfaces, AIO public header.
 * @copyright	VisionLabs LLC
 * @date     	25.06.2014
 * */

#pragma once

/** 
 * @mainpage LUNA SDK
 * @section IntroSection Introduction
 * Welcome to the FaceEngine reference manual! This documentations covers all public interfaces and types.
 *
 * \b Advanced \b usage
 * Advanced usage is covered in the Face Engine handbook. Please contact us via e-mail: info@visionlabs.ru 
 * if you haven't got one.
 *
 * \b Example \b code
 * Examples located at /examples folder of LUNA SDK package.
 *
 * @section StructureSection Structure
 * LUNA SDK consists of the following modules:
 *   \li Detectors: detect faces on images, and find landmarks on it
 *   \li Warper: normalize face position, scale and orientation by detection and landmarks.
 *   \li Estimators: estimate various properties of images such as blurriness.
 *   \li Descriptors: facilities to create and match face templates (aka descriptors).
 *   \li SDK core interfaces: common interfaces and macros shared across all SDK objects.
 *   \li Version: SDK Build type and version info.
 * */

#include <fsdk/IFaceEngineMobile.h>

namespace fsdk {

	using FaceEngineType = IFaceEngineMobile;
	using FaceEnginePtrType = IFaceEngineMobilePtr;

}
