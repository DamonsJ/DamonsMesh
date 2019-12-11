//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef _DPCSETTINGS_HEADER_
#define _DPCSETTINGS_HEADER_

//Qt
#include <QString>
#include <QStandardPaths>

//! Persistent settings key (to be used with QSettings)
class DPCSettings
{
public:
	static inline const QString OrganizationName			() { return "Damons"; }
	static inline const QString ApplicationName				() { return "DamonsPointCloud"; }
	static inline const QString DefaultDocPath() {
		return QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
	}

	static inline const QString AllFileFilters() { return "All (*.*)"; }
	static inline const QString FileFiltersSeparators() { return ";;"; }

	static inline const QString LoadFile                    () { return "LoadFile"; }
	static inline const QString SaveFile                    () { return "SaveFile"; }
	static inline const QString MainWinGeom                 () { return "mainWindowGeometry"; }
	static inline const QString MainWinState                () { return "mainWindowState"; }
	static inline const QString CurrentPath                 () { return "currentPath"; }
	static inline const QString SelectedInputFilter         () { return "selectedInputFilter"; }
	static inline const QString SelectedOutputFilterCloud   () { return "selectedOutputFilterCloud"; }
	static inline const QString SelectedOutputFilterMesh    () { return "selectedOutputFilterMesh"; }
	static inline const QString SelectedOutputFilterImage   () { return "selectedOutputFilterImage"; }
	static inline const QString SelectedOutputFilterPoly    () { return "selectedOutputFilterPoly"; }
	static inline const QString DuplicatePointsGroup        () { return "duplicatePoints"; }
	static inline const QString DuplicatePointsMinDist      () { return "minDist"; }
	static inline const QString HeightGridGeneration        () { return "HeightGridGeneration"; }
	static inline const QString VolumeCalculation			() { return "VolumeCalculation"; }
	static inline const QString Console                     () { return "Console"; }
	static inline const QString GlobalShift                 () { return "GlobalShift"; }
	static inline const QString MaxAbsCoord                 () { return "MaxAbsCoord"; }
	static inline const QString MaxAbsDiag                  () { return "MaxAbsDiag"; }
	static inline const QString AutoPickRotationCenter      () { return "AutoPickRotationCenter"; }
};

#endif// 2019/07/18

