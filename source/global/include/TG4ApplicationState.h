// $Id: TG4ApplicationState.h,v 1.4 2006/12/12 16:21:15 brun Exp $
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
  kInitGeometry,
  kAddParticles,
  kInEvent,
  kNotInApplication
};

#endif //TG4_APPLICATION_STATE_H
