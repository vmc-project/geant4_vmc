#ifndef TG4_G3_ATTRIBUTE_H
#define TG4_G3_ATTRIBUTE_H

// $Id$

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file TG4G3Attribute.h
/// \brief Definition of the enumeration TG4G3Attribute
///
/// \author A. Gheata

/// \ingroup visualization
/// \brief Enumeration type for G3 visualization attributes.

enum TG4G3Attribute
{
  /// Set the volume active/inactive for tracking (not used!)
  kWORK,    
      
  /// Set visibility
  /// -  0 = invisible  
  /// -  1 = visible
  /// - -1 = volume and daughters invisible,
  /// - -2 = volume visible but daughters invisible.
  kSEEN,
  
  /// Set line style
  /// - 1 = unbroken(default)
  /// - 2 = dashed
  /// - 3 = dotted
  /// - negative values = do the same for daughters
  kLSTY,
  
  /// Set line width
  kLWID,
  
  /// Set colour: 
  /// - 1-7 = G3 base colours(default=1):
  /// - 1 = black;  n=7+m, m=1,9, grey with increasing luminosity,
  /// - 2 = red;    n=17+m, m=1,25,
  /// - 3 = green;  n=67+m, m=1,25,
  /// - 4 = blue;   n=118+m, m=1,25,
  /// - 5 = yellow; n=42+m, m=1,25,
  /// - 6 = violet; n=142+m, m=1,25,
  /// - 7 = turquoise;  n=92+m, m=1,25,
  /// - negative values = do the same for daughters.
  kCOLO,
  
  /// Set fill style: 
  /// - 0 = forces drawing style to wireframe(default)
  /// - 1 = forces solid drawing style.
  /// - negative values = do the same for daughters.
  kFILL,
  
  /// Set number associated to volume name (not used!)
  kSET,
   
  /// Set detector number associated to volume name (not used!)
  kDET,
  
  /// Set detector type (not used!)
  kDTYP,
     
  /// Unknown attribute
  kUNKNOWN
};

#endif //TG4_G3_ATTRIBUTE_H
