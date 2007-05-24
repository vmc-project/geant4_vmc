// $Id: TG4ApplicationState.h,v 1.1 2007/02/06 11:06:34 brun Exp $
/// \ingroup global
//
/// \enum TG4ApplicationState
/// Enumeration for application states
///
/// Author: I. Hrivnacova

#ifndef TG4_APPLICATION_STATE_H
#define TG4_APPLICATION_STATE_H

enum TG4ApplicationState
{
  kPreInit,
  kConstructGeometry, 
  kConstructOpGeometry,
  kMisalignGeometry,
  kInitGeometry,
  kAddParticles,
  kInEvent,
  kNotInApplication
};

#endif //TG4_APPLICATION_STATE_H
