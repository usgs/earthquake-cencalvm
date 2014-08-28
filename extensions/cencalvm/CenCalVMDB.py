#!/usr/bin/env python
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# <LicenseText>
#
# ----------------------------------------------------------------------
#

## @file cencalvm/CenCalVMDB.py
##
## @brief Python manager for USGS Bay Area Velocty Model spatial
## database.
##
## Factory: spatial_database

from spatialdata.spatialdb.SpatialDBObj import SpatialDBObj
from cencalvm import CenCalVMDB as ModuleCenCalVMDB

# CenCalVMDB class
class CenCalVMDB(SpatialDBObj, ModuleCenCalVMDB):
  """
  Python manager for simple spatial database.

  Factory: spatial_database
  """

  # INVENTORY //////////////////////////////////////////////////////////

  class Inventory(SpatialDBObj.Inventory):
    """
    Python object for managing CenCalVMDB facilities and properties.
    """

    ## @class Inventory
    ## Python object for managing CenCalVMDB facilities and properties.
    ##
    ## \b Properties
    ## @li query_type Type of query to perform.
    ## @li query_res Resolution of query.
    ## @li filename Name of database file.
    ## @li cache_size Size of cache for database.
    ## @li filename_ext Name of extended database file.
    ## @li cache_size_ext Size of cache for extended database.
    ## @li \b squash Squash topography/bathymetry to sea level.
    ## @li \b squash_limit Elevation above which topography is squashed.
    ##
    ## \b Facilities
    ## @li None

    import pyre.inventory

    queryType = pyre.inventory.str("query_type", default="maxres")
    queryType.validator = pyre.inventory.choice(["maxres",
                                                 "fixedres",
                                                 "waveres"])
    queryType.meta['tip'] = "Type of query to perform."

    from pyre.units.length import m
    queryRes = pyre.inventory.dimensional("query_res", default=100.0*m)
    queryRes.meta['tip'] = "Resolution of query."

    filename = pyre.inventory.str("filename", default="")
    filename.meta['tip'] = "Name of database file."

    cacheSize = pyre.inventory.int("cache_size", default=128)
    cacheSize.meta['tip'] = "Size of cache for database."

    filenameExt = pyre.inventory.str("filename", default="")
    filenameExt.meta['tip'] = "Name of database file."

    cacheSizeExt = pyre.inventory.int("cache_size", default=128)
    cacheSizeExt.meta['tip'] = "Size of cache for database."

    squash = pyre.inventory.bool("squash", default=False)
    squash.meta['tip'] = "Squash topography/bathymetry to sea level."

    from pyre.units.length import km
    squashLimit = pyre.inventory.dimensional("squash_limit",
                                             default=-2.0*km)
    squashLimit.meta['tip'] = "Elevation above which topography is squashed."


  # PUBLIC METHODS /////////////////////////////////////////////////////

  def __init__(self, name="cencalvmdb"):
    """
    Constructor.
    """
    SpatialDBObj.__init__(self, name)
    return


  # PRIVATE METHODS ////////////////////////////////////////////////////

  def _configure(self):
    """
    Set members based on inventory.
    """
    SpatialDBObj._configure(self)

    if self.inventory.queryType == 'maxres':
      self.queryType(self.MAXRES)
    elif self.inventory.queryType == 'fixedres':
      self.queryType(self.MAXRES)
    elif self.inventory.queryType == 'waveres':
      self.queryType(self.WAVERES)
    else:
      raise ValueError("Unknown queryType '%s'." % self.inventory.queryType)

    self.queryRes(self.inventory.queryRes.value)
    self.filename(self.inventory.filename)
    self.cacheSize(self.inventory.cacheSize)
    self.filenameExt(self.inventory.filenameExt)
    self.cacheSizeExt(self.inventory.cacheSizeExt)
    self.squash(self.inventory.squash, self.inventory.squashLimit.value)
    return


  def _createModuleObj(self):
    """
    Create Python module object.
    """
    ModuleCenCalVMDB.__init__(self)
    return


# FACTORIES ////////////////////////////////////////////////////////////

def spatial_database():
  """
  Factory associated with CenCalVMDB.
  """
  return CenCalVMDB()


# End of file 
