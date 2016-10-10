
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAByteMap2D.h
// Do not include this file directly! It will automatically be included when
// including "NAByteMap2D.h"


#include "NAMemory.h"
#include "NABinaryData.h"


struct NAMapi{
  NARecti rect;       // The origin and size of the map
  void* data;       // the pointer to the data;
};


struct NAMap3i{
  NABoxi box;       // The origin and size of the map
  void* data;       // the pointer to the data;
};


struct NAByteMap2D{
  NAMapi map;
};
// The two size fields (width and height) denote the number of entries in the
// primary and secondary dimension. Note that there is no definition of how
// these dimensions shall be semantically interpreted in this struct. You
// therefore don't know if the primary dimension is horizontal or vertical or
// if the vertical axis goes from top to bottom or from bottom to top. You can
// use NACoordSysi (todo) for that. But note that a 2D array is always stored
// primary dimension first, just like a regular C-style array.
//
// An empty size (naIsSizeiEmpty) is perfectly valid but explicitely denotes
// an empty map.
//
// Implementation notice: When rect is empty (naIsSizeiEmpty), the map is
// considered empty. In that case, the other fields contain no useful
// information.
//
// Negative or otherwise invalid rects or sizes behave undefined and may crash
// your application!



NA_IDEF NAByteMap2D* naInitByteMap2D(NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naInitByteMap2DWithRectiCopyingBuffer", "map2d is Null-Pointer"); return NA_NULL;}
  #endif
  map2d->map.rect.size.width  = 0;
  map2d->map.rect.size.height = 0;
  return map2d;
}


NA_IDEF NAByteMap2D* naInitByteMap2DWithRecti(NAByteMap2D* map2d,
                                                     NARecti rect){
  // Declaration before implementation. Needed for C90.
  NAInt totalsize;
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naInitByteMap2DWithRectiCopyingBuffer", "map2d is Null-Pointer"); return NA_NULL;}
    if(!naIsRectiValid(rect))
      naError("naInitByteMap2DWithRecti", "rect is invalid.");
  #endif
  totalsize = naGetRectiIndexCount(rect);
  if(!totalsize){  // if total size is zero
    map2d = naInitByteMap2D(map2d);
  }else{
    map2d->map.rect = rect;
    #ifndef NDEBUG
      if(naIsRectiValid(rect) && (totalsize < 0))
        naError("naInitByteMap2DWithRecti", "Total size exceeds int range");
    #endif
    map2d->map.data = (NAByte*)naMalloc(totalsize);
    naNulln(map2d->map.data, totalsize);
  }
  return map2d;
}


NA_IDEF NAByteMap2D* naInitByteMap2DWithRectiCopyingBuffer(
                                                      NAByteMap2D* map2d,
                                                           NARecti rect,
                                                    const NAByte* buffer){
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naInitByteMap2DWithRectiCopyingBuffer", "map2d is Null-Pointer"); return NA_NULL;}
    if(!buffer){
      naCrash("naInitByteMap2DWithRectiCopyingBuffer", "buffer is Null-Pointer.");
      return NA_NULL;
    }
  #endif
//  if(naIsRectiEmpty(rect)){return naInitByteMap2D(map2d);}
  map2d = naInitByteMap2DWithRecti(map2d, rect);
  if(!naIsByteMap2DEmpty(map2d)){
    naCopyn(map2d->map.data, buffer, rect.size.width * rect.size.height);
  }
  return map2d;
}


NA_IDEF void naClearByteMap2D(NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naClearByteMap2D", "map2d is Null-Pointer.");
      return;
    }
  #endif
  if(!naIsRectiEmpty(map2d->map.rect)){free(map2d->map.data);}
}


NA_IDEF void naEmptyByteMap2D(NAByteMap2D* map2d){
  // You could add a test if the map is empty but it is expected that this
  // function will be called more often with non-empty maps.
  naClearByteMap2D(map2d);
  naInitByteMap2D(map2d);
}



NA_IDEF void naEnhanceByteMap2DWithRecti(NAByteMap2D* map2d, NARecti rect){
  // Declaration before implementation. Needed for C90.
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naEnhanceByteMap2DWithRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naEnhanceByteMap2DWithRecti",  "rect of map2d is invalid. "
                                                "Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naEnhanceByteMap2DWithRecti", "rect is invalid.");
    }
  #endif
  if(naIsRectiEmpty(map2d->map.rect)){
    // the storage is empty. Create a new one.
    naClearByteMap2D(map2d);
    naInitByteMap2DWithRecti(map2d, rect);
  }else{
    NARecti enhancedrect = naMakeRectiWithRectiAndRecti(map2d->map.rect, rect);  
    NAInt totalsize = naGetRectiIndexCount(enhancedrect);
    #ifndef NDEBUG
      if(!naIsRectiUseful(enhancedrect))
        naError("naEnhanceByteMap2DWithRecti", "enhanced rect is invalid");
      if(totalsize <= 0){
        naCrash("naEnhanceByteMap2DWithRecti", "Total size %" NA_PRIi " is invalid", totalsize);
        return;
      }
    #endif


    if(!naEqualRecti(enhancedrect, map2d->map.rect)){
      // If the enhanced rect is different than the current rect, enhance!
      NAByte* olddataptr = map2d->map.data;
      NAByte* newdata = naMalloc(totalsize);
      NAByte* newdataptr = newdata;
      
      // Compute the bounds.
      NAInt leadbound1 =     map2d->map.rect.pos.x
                           - enhancedrect.pos.x;
      NAInt trailbound1 =    naGetRectiEndX(enhancedrect)
                           - naGetRectiEndX(map2d->map.rect);
      NAInt leadbound2 =    (map2d->map.rect.pos.y - enhancedrect.pos.y)
                           * enhancedrect.size.width;
      NAInt trailbound2 =   (naGetRectiEndY(enhancedrect)
                           - naGetRectiEndY(map2d->map.rect))
                           * enhancedrect.size.width;
      // The bounds of dimension 2 are premultiplied with the width.
      
      // Fill the contents.
      // Fill the top bound with binary zero.
      if(leadbound2){
        naNulln(newdataptr, leadbound2);
        newdataptr += leadbound2;
      }
      // Fill the middle
      if(leadbound1 || trailbound1){
        // The map has been horizontally enhanded. We need to copy line by line.
        for(v2 = 0; v2 < map2d->map.rect.size.height; v2++){
          if(leadbound1){
            naNulln(newdataptr, leadbound1);
            newdataptr += leadbound1;
          }
          naCopyn(newdataptr, olddataptr, map2d->map.rect.size.width);
          newdataptr += map2d->map.rect.size.width;
          olddataptr += map2d->map.rect.size.width;
          if(trailbound1){
            naNulln(newdataptr, trailbound1);
            newdataptr += trailbound1;
          }
        }
      }else{
        // The map has NOT been horizontally enhanced. Just copy the whole old
        // data block to the new one.
        NAInt totaloldsize = naGetRectiIndexCount(map2d->map.rect);
        naCopyn(newdataptr, olddataptr, totaloldsize);
        newdataptr += totaloldsize;
//        olddataptr += totaloldsize;
      }
      // Fill the bottom bound with binary zero.
      if(trailbound2){
        naNulln(newdataptr, trailbound2);
//        newdataptr += trailbound2;
      }
            
      // Delete the old data and attach the new one.
      free(map2d->map.data);
      map2d->map.data = newdata;
      map2d->map.rect = enhancedrect;
    }
  }
  return;
}


NA_IDEF void naClampByteMap2DToRecti(NAByteMap2D* map2d, NARecti rect){
  NAByteMap2D tmpbytemap; // declaration before implementation. Needed for C90
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naClampByteMap2DToRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naClampByteMap2DToRecti", "rect of map2d is invalid. "
                                            "Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naClampByteMap2DToRecti", "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naClampByteMap2DToRecti", "rect is not contained in map rect.");
    }
  #endif
  naInitByteMap2DWithRecti(&tmpbytemap, rect);
  naFillByteMap2DWithByteMapInRecti(&tmpbytemap, rect, map2d);
  free(map2d->map.data);
  map2d->map.data = tmpbytemap.map.data;
  map2d->map.rect = rect;
}


NA_IDEF NAByte* naEnhanceByteMap2DAtPosi(NAByteMap2D* map2d, NAPosi pos){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naEnhanceByteMap2DAtPosi", "map2d is Null-Pointer.");
      return NA_NULL;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naEnhanceByteMap2DAtPosi", "rect of map2d is invalid. "
                                            "Uninitialized struct?");
      if(!naIsPosiValid(pos))
        naError("naEnhanceByteMap2DAtPosi", "pos is invalid.");
    }
  #endif
  // todo: make own implementation
  naEnhanceByteMap2DWithRecti(map2d, naMakeRecti(pos, naMakeSizei(1, 1)));
  return naGetByteMap2DMutableByte(map2d, pos);
}


NA_IDEF void naFillByteMap2DWithValueInRecti( NAByteMap2D* map2d,
                                                   NARecti rect,
                                                    NAByte value){
  // Declaration before implementation. Needed for C90.
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithValueInRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naFillByteMap2DWithValueInRecti",  "rect of map2d is invalid. "
                                                    "Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naFillByteMap2DWithValueInRecti", "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naFillByteMap2DWithValueInRecti", "rect is not fully on map.");
    }
  #endif
  if(rect.size.width == map2d->map.rect.size.width){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, rect.pos);
    memset(dstptr, value, naGetRectiIndexCount(rect));
  }else{
    NAPosi leadpos = rect.pos;
    for(v2 = 0; v2 < rect.size.height; v2++){
      NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
      memset(dstptr, value, rect.size.width);
      leadpos.y++;
    }
  }
}


NA_IDEF void naFillByteMap2DWithValueInMaskedRecti(   NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                            NAByte value){
  // Declaration before implementation. Needed for C90.
  NAPosi leadpos;
  NAInt v1;
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithValueInMaskedRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naFillByteMap2DWithValueInMaskedRecti",
                "rect of map2d is invalid. Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naFillByteMap2DWithValueInMaskedRecti",
                "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naFillByteMap2DWithValueInMaskedRecti",
                "rect is not fully on map.");
    }
    if(!mask){
      naCrash("naFillByteMap2DWithValueInMaskedRecti", "mask is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(mask->map.rect))
        naError("naFillByteMap2DWithValueInMaskedRecti",
                "rect of mask is invalid. Uninitialized struct?");
      if(!naIsRectiInRecti(rect, mask->map.rect))
        naError("naFillByteMap2DWithValueInMaskedRecti",
                "rect is not fully in mask.");
    }
  #endif
  leadpos = rect.pos;
  for(v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* maskptr = naGetByteMap2DConstByte(mask, leadpos);
    for(v1 = 0; v1 < rect.size.width; v1++){
      if(*maskptr){*dstptr = value;}
      dstptr++;
      maskptr++;
    }
    leadpos.y++;
  }
}


NA_IDEF void naFillByteMap2DWithByteMapInMaskedRecti( NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                const NAByteMap2D* values){
  // Declaration before implementation. Needed for C90.
  NAPosi leadpos;
  NAInt v1;
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect of map2d is invalid. Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is not fully on map.");
    }
    if(!mask){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti", "mask is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(mask->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect of mask is invalid. Uninitialized struct?");
      if(!naIsRectiInRecti(rect, mask->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is not fully in mask.");
    }
    if(!values){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti", "values is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(values->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect of values is invalid. Uninitialized struct?");
      if(!naIsRectiInRecti(rect, values->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is not fully in values.");
    }
  #endif
  leadpos = rect.pos;
  for(v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* maskptr = naGetByteMap2DConstByte(mask, leadpos);
    const NAByte* valueptr = naGetByteMap2DConstByte(values, leadpos);
    for(v1 = 0; v1 < rect.size.width; v1++){
      if(*maskptr){*dstptr = *valueptr;}
      dstptr++;
      maskptr++;
      valueptr++;
    }
    leadpos.y++;
  }
}


NA_IDEF void naFillByteMap2DWithByteMapInRecti(NAByteMap2D* map2d,
                                                    NARecti rect,
                                         const NAByteMap2D* values){
  // Declaration before implementation. Needed for C90.
  NAPosi leadpos;
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti", "map2d is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(map2d->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect of map2d is invalid. Uninitialized struct?");
      if(!naIsRectiValid(rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is not fully on map.");
    }
    if(!values){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti", "values is Null-Pointer.");
      return;
    }else{
      if(!naIsRectiValid(values->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect of values is invalid. Uninitialized struct?");
      if(!naIsRectiInRecti(rect, values->map.rect))
        naError("naFillByteMap2DWithByteMapInMaskedRecti",
                "rect is not fully in values.");
    }
  #endif
  leadpos = rect.pos;
  for(v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* valueptr = naGetByteMap2DConstByte(values, leadpos);
    naCopyn(dstptr, valueptr, rect.size.width);
    leadpos.y++;
  }
}


NA_IDEF void naReplaceByteMap2DValueWithValue( NAByteMap2D* map2d,
                                                    NARecti rect,
                                                     NAByte originalvalue,
                                                     NAByte newvalue){
  // Declaration before implementation. Needed for C90.
  NAPosi leadpos;
  NAInt v1;
  NAInt v2;
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naReplaceByteMap2DValueWithValue", "map2d is Null-Pointer.");
      return;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naReplaceByteMap2DValueWithValue", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naReplaceByteMap2DValueWithValue", "rect of map2d is invalid. "
                                                    "Uninitialized struct?");
      if(naIsRectiEmpty(rect))
        naError("naReplaceByteMap2DValueWithValue", "rect is empty.");
      if(!naIsRectiValid(rect))
        naError("naReplaceByteMap2DValueWithValue", "rect is invalid.");
      if(!naIsRectiInRecti(rect, map2d->map.rect))
        naError("naReplaceByteMap2DValueWithValue", "rect is not fully in map.");
    }
  #endif
  leadpos = rect.pos;
  for(v2=0; v2<rect.size.height; v2++){
    NAByte* dataptr = naGetByteMap2DMutableByte(map2d, leadpos);
    for(v1=0; v1<rect.size.width; v1++){
      if(*dataptr == originalvalue){*dataptr = newvalue;}
      dataptr++;
    }
    leadpos.y++;
  }
}


NA_IDEF const NAByte* naGetByteMap2DConstPointer(const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DConstPointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naGetByteMap2DConstPointer", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naGetByteMap2DConstPointer",  "rect of map2d is invalid. "
                                               "Uninitialized struct?");
    }
  #endif
  return map2d->map.data;
}


NA_IDEF NAByte* naGetByteMap2DMutablePointer(NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DMutablePointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naGetByteMap2DMutablePointer", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naGetByteMap2DMutablePointer",  "rect of map2d is invalid. "
                                                 "Uninitialized struct?");
    }
  #endif
  return map2d->map.data;
}



NA_IDEF const NAByte* naGetByteMap2DConstByte(const NAByteMap2D* map2d,
                                                          NAPosi pos){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DConstByte", "map2d is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naGetByteMap2DConstByte", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naGetByteMap2DConstByte", "rect of map2d is invalid. "
                                           "Uninitialized struct?");
      if(!naIsPosiInRecti(pos, map2d->map.rect))
        naError("naGetByteMap2DConstByte", "pos outside of map");
    }
  #endif
  return &(((NAByte*)(map2d->map.data))[naGetRectiIndexOfPosiRowFirst(map2d->map.rect, pos)]);
}


NA_IDEF NAByte* naGetByteMap2DMutableByte(NAByteMap2D* map2d, NAPosi pos){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DMutableByte", "map2d is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naGetByteMap2DMutableByte", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naGetByteMap2DMutableByte", "rect of map2d is invalid. "
                                           "Uninitialized struct?");
      if(!naIsPosiInRecti(pos, map2d->map.rect))
        naError("naGetByteMap2DMutableByte", "pos outside of map");
    }
  #endif
//  indx = naGetRectiIndexOfPosi(map2d->rect, pos);
  return &(((NAByte*)(map2d->map.data))[naGetRectiIndexOfPosiRowFirst(map2d->map.rect, pos)]);
}



NA_IDEF NABool naIsPosiInByteMap2D(NAPosi pos, const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naIsPosiInByteMap2D", "map2d is Null-Pointer.");
      return NA_FALSE;
    }else{
      if(naIsRectiEmpty(map2d->map.rect))
        naError("naIsPosiInByteMap2D", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naIsPosiInByteMap2D", "rect of map2d is invalid. "
                                           "Uninitialized struct?");
    }
  #endif
  return naIsPosiInRecti(pos, map2d->map.rect);
}


NA_IDEF NARecti naGetByteMap2DRecti(const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      // Code sanity checks require to return a valid value. But C90 does not
      // allow const literals. Therefore, we initialize a dummy struct.
      NARecti dummyrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(1, 1));
      naCrash("naGetByteMap2DRect", "map2d is Null-Pointer.");
      return dummyrect;
    }else{
      // Note that the following empty-check has been removed. It is a warning
      // which is very annoying. At first, there existed a separate function
      // naIsByteMap2DEmpty to solve that but the author himself found this to
      // be very stupid and it lead to bad programming style. If an empty rect
      // is used later on, errors will be emitted anyway.
      // if(naIsRectiEmpty(map2d->rect))
      //   naError("naGetByteMap2DRect", "map2d is Empty.");
      if(!naIsRectiValid(map2d->map.rect))
        naError("naGetByteMap2DRect", "rect of map2d is invalid. "
                                           "Uninitialized struct?");
    }
  #endif
  return map2d->map.rect;
}


NA_IDEF NABool naIsByteMap2DEmpty(const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naIsByteMap2DEmpty", "map2d is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return naIsRectiEmpty(map2d->map.rect);
}



NA_IDEF void naMoveByteMap2DToPosi(NAByteMap2D* map2d, NAPosi pos){
  map2d->map.rect.pos = pos;
}


NA_IDEF void naMoveByteMap2DBySizei(NAByteMap2D* map2d, NASizei size){
  map2d->map.rect.pos = naAddPosiSizei(map2d->map.rect.pos, size);
}





// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


