// $Id: TG4ElementTable.h,v 1.1.1.1 2002/09/27 10:00:03 rdm Exp $
/// \ingroup geometry
//
/// \class TG4ElementTable
/// 
/// Singleton class that builds the default element table
/// Currently defined elements: up to Z=47 (to be continued).
/// Not used.
///
/// Author: I. Hrivnacova 

#ifndef TG4_ELEMENT_TABLE_H
#define TG4_ELEMENT_TABLE_H

class TG4ElementTable 
{
  public:
    // --> protected
    // TG4ElementTable();
    // TG4ElementTable(const TG4ElementTable& right);
    virtual ~TG4ElementTable();
    
    // static methods
    static TG4ElementTable* Instance();

  protected:
    TG4ElementTable();    
    TG4ElementTable(const TG4ElementTable& right); 
    
    // operators
    TG4ElementTable& operator=(const TG4ElementTable& right);
          
  private:
    void Construct() const;

    // data members
    static TG4ElementTable*  fgInstance; //this instance
};   

#endif //G4_ELEMENT_TABLE_H
