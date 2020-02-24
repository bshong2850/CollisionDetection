#pragma once
#ifndef CORE_H
#define CORE_H

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
//Collision Detection
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCollisionDetectionFilter.h"
#include "vtkMatrix4x4.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkTransform.h"
#include <vtkTimerLog.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>

#include <vtkSTLReader.h>
#include <vtkPLYReader.h>
#include <vtkOBJReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataReader.h>
#include <vtkBYUReader.h>
#include <vtkSphereSource.h>
#include <vtksys/SystemTools.hxx>
#include <vtkRegularPolygonSource.h>

#include "Keyboard.h"

//SAMPLE

#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <iostream>


class vtkFPSCallback : public vtkCommand
{
public:
	static vtkFPSCallback *New()
	{
		return new vtkFPSCallback;
	}

	void SetTextActor(vtkTextActor *txt)
	{
		this->TextActor = txt;
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		static vtkTimerLog* timer = vtkTimerLog::New();
		timer->StopTimer();
		double fps = 1.0 / timer->GetElapsedTime();
		timer->StartTimer();

		// output to caller object
		vtkRenderer *ren = reinterpret_cast<vtkRenderer *>(caller);
		float render_time = ren->GetLastRenderTimeInSeconds();
		sprintf(this->TextBuff, "fps %.1f", fps);
		this->TextActor->SetInput(this->TextBuff);
	}
protected:
	vtkTextActor *TextActor;
	char TextBuff[128];
};


class vtkSaveVertexCallback : public vtkCommand
{
public:
	int _count = 0;
	static vtkSaveVertexCallback *New()
	{
		return new vtkSaveVertexCallback;
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkTransformPolyDataFilter *TPDF = reinterpret_cast<vtkTransformPolyDataFilter *>(caller);
		
		int numberofpoints = TPDF->GetOutput()->GetNumberOfPoints();
		vtkPoints* inPts = TPDF->GetOutput()->GetPoints();

		std::ofstream writeFile;
		if(numberofpoints > 0)
		{
			writeFile.open("result/Line/" + std::to_string(_count) + ".bv");

			if (writeFile.is_open())
			{
				for (int i = 0; i < numberofpoints; i++)
				{
					double p[3];
					inPts->GetPoint(i, p);

					writeFile << p[0] << " " << p[1] << " " << p[2] << std::endl;
				}
			}
			writeFile.close();
			_count++;
		}
	}
protected:
};



class VTKCore : public vtkFPSCallback, public vtkSaveVertexCallback
{
private:
	vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
	vtkSmartPointer<vtkCollisionDetectionFilter> MeshToMeshCollisionDetect(
		int contactMode,
		vtkSmartPointer<vtkPolyData> polyData1,
		vtkSmartPointer<vtkPolyData> polyData2,
		vtkSmartPointer<vtkMatrix4x4> &polyData1_Matrix,
		vtkSmartPointer<vtkMatrix4x4> &polyData2_Matrix
	);

	void ComputeCenter(double *bounds, double &center_x, double &center_y, double &center_z);
protected:


public:
	VTKCore();
	~VTKCore();

	void PrintKeyFunction();
	
	void CollisionDetection();

};






#endif