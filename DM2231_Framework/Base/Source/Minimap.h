#ifndef MINIMAP_H
#define MINIMAP_H

#include "Mesh.h"

class CMinimap
{
public:
	CMinimap(void);
	virtual ~CMinimap(void);

	Mesh *m_cMinimap_Background, *m_cMinimap_Border, *m_cMinimap_Avatar;

	// Set and get background mesh
	bool SetBackground(Mesh *aBackground);
	Mesh *GetBackground();

	// Set the Border mesh to this class
	bool SetBorder(Mesh* aBorder);
	// Get the Border mesh to this class 
 	Mesh* GetBorder(void); 
 	// Set the Avatar mesh to this class
	bool SetAvatar(Mesh* anAvatar);
	// Get the Avatar mesh to this class 
 	Mesh* GetAvatar(void);

	// Set angle of avatar
	bool SetAngle(const int angle); 
 	// Get angle
	int GetAngle(void); 
 	// Set position of avatar in minimap
	bool SetPosition(const int x, const int y);
	// Get position x of avatar in minimap
	int GetPosition_x(void); 
 	// Get position y of avatar in minimap
	int GetPosition_y(void); 
 	// Set size of minimap (for calculation of avatar in minimap)
	bool SetSize(const int size_x, const int size_y); 
 	// Get size of minimap (for calculation of avatar in minimap)
	int GetSize_x(void); 
 	// Get size of minimap (for calculation of avatar in minimap)
	int GetSize_y(void); 

 private: 
 	// Rotation from First Angle
	 int angle; 
 	// Offset in the minimap
	 int x, y; 
 	// Minimap size
	 int size_x, size_y; 

};

#endif