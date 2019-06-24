#!/usr/bin/env python

# ----------------------------------------------------------------------
# tools configuration file "tools.py" for the
# saga_cmd parameter interface generator "param_interface.py"

# copyright (C) 2015 by Volker Wichmann
# released under the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.

# purpose:
# this is the list of SAGA tools for which the parameter interface
# should be created
#
# usage:
# the list of tools has a STRICT ORDER:
# * new tools (and libraries) have to be appended at the end
# * tools renamed have to be edited in place
# usually you do not need to edit this list because for a specific
# SAGA version as the "param_interface.py" generator silently skips
# tools that are not found.
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
tools = [
['climate_tools', 0, 'Multi Level to Surface Interpolation'],
['climate_tools', 1, 'Multi Level to Points Interpolation'],
['climate_tools', 2, 'Earth\'s Orbital Parameters'],
['climate_tools', 3, 'Annual Course of Daily Insolation'],
['climate_tools', 4, 'Daily Insolation over Latitude'],
['climate_tools', 5, 'Monthly Global by Latitude'],
['climate_tools', 6, 'ETpot (after Hargreaves, Table)'],
['climate_tools', 7, 'Daily to Hourly ETpot'],
['contrib_perego', 0, 'Average With Thereshold 1'],
['contrib_perego', 1, 'Average With Thereshold 2'],
['contrib_perego', 2, 'Average With Thereshold 3'],
['contrib_perego', 3, 'Average With Mask 1'],
['contrib_perego', 4, 'Average With Mask 2'],
['contrib_perego', 5, 'Destriping'],
['contrib_perego', 6, 'Destriping with Mask'],
['contrib_perego', 7, 'Directional Average'],
['db_odbc', 0, 'Connect to ODBC Source'],
['db_odbc', 1, 'Disconnect from ODBC Source'],
['db_odbc', 2, 'Commit/Rollback Transaction'],
['db_odbc', 3, 'Execute SQL'],
['db_odbc', 4, 'List Table Fields'],
['db_odbc', 5, 'Import Table'],
['db_odbc', 6, 'Export Table'],
['db_odbc', 7, 'Drop Table'],
['db_odbc', 8, 'Import Table from SQL Query'],
['db_odbc', 9, 'List ODBC Servers'],
['db_odbc', 10, 'List Tables'],
['db_odbc', 11, 'Disconnect All'],
['db_pgsql', 0, 'List PostgreSQL Connections'],
['db_pgsql', 1, 'Connect to PostgreSQL'],
['db_pgsql', 2, 'Disconnect from PostgreSQL'],
['db_pgsql', 3, 'Disconnect All'],
['db_pgsql', 4, 'Begin Transaction'],
['db_pgsql', 5, 'Commit/Rollback Transaction'],
['db_pgsql', 6, 'Execute SQL'],
['db_pgsql', 10, 'List Tables'],
['db_pgsql', 11, 'List Table Fields'],
['db_pgsql', 12, 'Import Table'],
['db_pgsql', 13, 'Export Table'],
['db_pgsql', 14, 'Drop Table'],
['db_pgsql', 15, 'Import Table from SQL Query'],
['db_pgsql', 20, 'Import Shapes from PostGIS'],
['db_pgsql', 21, 'Export Shapes to PostGIS'],
['db_pgsql', 22, 'Update Shapes SRID'],
['db_pgsql', 30, 'Import Raster from PostGIS'],
['db_pgsql', 31, 'Export Raster to PostGIS'],
['db_pgsql', 32, 'Update Raster SRID'],
['docs_html', 1, 'SVG Interactive Map'],
['docs_pdf', 0, 'Shapes Report'],
['docs_pdf', 1, 'Shapes Summary Report'],
['docs_pdf', 2, 'Terrain Path Cross Sections'],
['garden_3d_viewer', 0, 'TIN Viewer'],
['garden_3d_viewer', 1, 'Point Cloud Viewer'],
['garden_3d_viewer', 2, '3D Shapes Viewer'],
['garden_3d_viewer', 3, 'Globe Viewer for Grids'],
['garden_3d_viewer', 4, 'Multiple Grids Viewer'],
['garden_fractals', 0, 'Bifurcation'],
['garden_fractals', 1, 'Pythagoras\' Tree'],
['garden_fractals', 3, 'Fractal Dimension of Grid Surface'],
['garden_fractals', 5, 'Gaussian Landscapes'],
['garden_learn_to_program', 0, '01: My first module'],
['garden_learn_to_program', 1, '02: Pixel by pixel operations with two grids'],
['garden_learn_to_program', 2, '03: Direct neighbours'],
['garden_learn_to_program', 3, '04: Direct neighbours - more...'],
['garden_learn_to_program', 4, '05: Direct neighbours - slope and aspect'],
['garden_learn_to_program', 5, '06: Extended neighbourhoods'],
['garden_learn_to_program', 6, '07: Extended neighbourhoods - catchment areas (trace flow)'],
['garden_learn_to_program', 7, '08: Extended neighbourhoods - catchment areas (parallel)'],
['garden_learn_to_program', 8, '09: Extended neighbourhoods - catchment areas (recursive)'],
['garden_learn_to_program', 9, '10: Dynamic Simulation - Life'],
['garden_learn_to_program', 10, '11: Dynamic Simulation - Soil Nitrogen Dynamics'],
['garden_learn_to_program', 11, '12: First steps with shapes'],
['garden_learn_to_program', 12, '13: Reprojecting a shapes layer'],
['garden_learn_to_program', 13, '14: Vectorising channel lines'],
['garden_webservices', 0, 'Import a Map via Web Map Service (WMS)'],
['grid_analysis', 0, 'Accumulated Cost'],
['grid_analysis', 1, 'Accumulated Cost (Anisotropic)'],
['grid_analysis', 5, 'Least Cost Paths'],
['grid_analysis', 6, 'Change Vector Analysis'],
['grid_analysis', 7, 'Covered Distance'],
['grid_analysis', 8, 'Pattern Analysis'],
['grid_analysis', 9, 'Layer of extreme value'],
['grid_analysis', 10, 'Analytical Hierarchy Process'],
['grid_analysis', 11, 'Ordered Weighted Averaging'],
['grid_analysis', 12, 'Aggregation Index'],
['grid_analysis', 13, 'Cross-Classification and Tabulation'],
['grid_analysis', 14, 'Soil Texture Classification'],
['grid_analysis', 15, 'Fragmentation (Standard)'],
['grid_analysis', 16, 'Fragmentation (Alternative)'],
['grid_analysis', 17, 'Fragmentation Classes from Density and Connectivity'],
['grid_analysis', 18, 'Accumulation Functions'],
['grid_analysis', 19, 'IMCORR - Feature Tracking'],
['grid_analysis', 20, 'Soil Texture Classification for Tables'],
['grid_calculus', 0, 'Grid Normalization'],
['grid_calculus', 1, 'Grid Calculator'],
['grid_calculus', 2, 'Grid Volume'],
['grid_calculus', 3, 'Grid Difference'],
['grid_calculus', 4, 'Function Plotter'],
['grid_calculus', 5, 'Geometric Figures'],
['grid_calculus', 6, 'Random Terrain'],
['grid_calculus', 7, 'Random Field'],
['grid_calculus', 8, 'Grids Sum'],
['grid_calculus', 9, 'Grids Product'],
['grid_calculus', 10, 'Grid Standardization'],
['grid_calculus', 11, 'Fuzzify'],
['grid_calculus', 12, 'Fuzzy Intersection (AND)'],
['grid_calculus', 13, 'Fuzzy Union (OR)'],
['grid_calculus', 14, 'Metric Conversions'],
['grid_calculus', 15, 'Gradient Vector from Cartesian to Polar Coordinates'],
['grid_calculus', 16, 'Gradient Vector from Polar to Cartesian Coordinates'],
['grid_calculus', 18, 'Grid Division'],
['grid_calculus', 19, 'Spherical Harmonic Synthesis'],
['grid_calculus_bsl', 0, 'BSL'],
['grid_calculus_bsl', 1, 'BSL from File'],
['grid_filter', 0, 'Simple Filter'],
['grid_filter', 1, 'Gaussian Filter'],
['grid_filter', 2, 'Laplacian Filter'],
['grid_filter', 3, 'Multi Direction Lee Filter'],
['grid_filter', 4, 'User Defined Filter'],
['grid_filter', 5, 'Filter Clumps'],
['grid_filter', 6, 'Majority/Minority Filter'],
['grid_filter', 7, 'DTM Filter (slope-based)'],
['grid_filter', 8, 'Morphological Filter'],
['grid_filter', 9, 'Rank Filter'],
['grid_filter', 10, 'Mesh Denoise'],
['grid_filter', 11, 'Resampling Filter'],
['grid_filter', 12, 'Geodesic Morphological Reconstruction'],
['grid_filter', 13, 'Binary Erosion-Reconstruction'],
['grid_filter', 14, 'Connectivity Analysis'],
['grid_filter', 15, 'Sieve Classes'],
['grid_gridding', 0, 'Shapes to Grid'],
['grid_gridding', 1, 'Inverse Distance Weighted'],
['grid_gridding', 2, 'Nearest Neighbour'],
['grid_gridding', 3, 'Natural Neighbour'],
['grid_gridding', 4, 'Modifed Quadratic Shepard'],
['grid_gridding', 5, 'Triangulation'],
['grid_gridding', 6, 'Kernel Density Estimation'],
['grid_gridding', 7, 'Angular Distance Weighted'],
['grid_spline', 1, 'Thin Plate Spline'],
['grid_spline', 2, 'Thin Plate Spline (TIN)'],
['grid_spline', 3, 'B-Spline Approximation'],
['grid_spline', 4, 'Multilevel B-Spline'],
['grid_spline', 5, 'Multilevel B-Spline from Grid Points'],
['grid_spline', 6, 'Cubic Spline Approximation'],
['grid_spline', 7, 'Multilevel B-Spline for Categories'],
['grid_tools', 0, 'Resampling'],
['grid_tools', 1, 'Aggregate'],
['grid_tools', 3, 'Mosaicking'],
['grid_tools', 4, 'Constant Grid'],
['grid_tools', 5, 'Patching'],
['grid_tools', 6, 'Close One Cell Gaps'],
['grid_tools', 7, 'Close Gaps'],
['grid_tools', 8, 'Grid Buffer'],
['grid_tools', 9, 'Threshold Buffer'],
['grid_tools', 10, 'Grid Proximity Buffer'],
['grid_tools', 11, 'Change Data Storage'],
['grid_tools', 12, 'Change Grid Values'],
['grid_tools', 15, 'Reclassify Grid Values'],
['grid_tools', 17, 'Crop to Data'],
['grid_tools', 18, 'Invert Data/No-Data'],
['grid_tools', 19, 'Grid Orientation'],
['grid_tools', 20, 'Combine Grids'],
['grid_tools', 21, 'Grid Cell Index'],
['grid_tools', 22, 'Grids from classified grid and table'],
['grid_tools', 23, 'Create Grid System'],
['grid_tools', 24, 'Grid Masking'],
['grid_tools', 25, 'Close Gaps with Spline'],
['grid_tools', 26, 'Proximity Grid'],
['grid_tools', 27, 'Tiling'],
['grid_tools', 28, 'Shrink and Expand'],
['grid_tools', 29, 'Close Gaps with Stepwise Resampling'],
['grid_tools', 30, 'Transpose Grids'],
['grid_tools', 31, 'Clip Grids'],
['grid_visualisation', 0, 'Color Palette Rotation'],
['grid_visualisation', 1, 'Grid Animation'],
['grid_visualisation', 2, 'Fit Color Palette to Grid Values'],
['grid_visualisation', 3, 'RGB Composite'],
['grid_visualisation', 4, 'Create 3D Image'],
['grid_visualisation', 5, 'Color Triangle Composite'],
['grid_visualisation', 6, 'Histogram Surface'],
['grid_visualisation', 7, 'Aspect-Slope Grid'],
['grid_visualisation', 8, 'Terrain Map View'],
['grid_visualisation', 9, 'Split RGB Composite'],
['imagery_classification', 0, 'Supervised Classification for Grids'],
['imagery_classification', 1, 'K-Means Clustering for Grids'],
['imagery_classification', 2, 'Confusion Matrix (Two Grids)'],
['imagery_classification', 3, 'Decision Tree'],
['imagery_classification', 4, 'Supervised Classification for Shapes'],
['imagery_classification', 5, 'Supervised Classification for Tables'],
['imagery_opencv', 0, 'Morphological Filter (OpenCV)'],
['imagery_opencv', 1, 'Fourier Transformation (OpenCV)'],
['imagery_opencv', 2, 'Single Value Decomposition (OpenCV)'],
['imagery_opencv', 3, 'Neural Networks (OpenCV)'],
['imagery_opencv', 4, 'Stereo Match (OpenCV)'],
['imagery_photogrammetry', 0, 'Resection (Terrestrial)'],
['imagery_photogrammetry', 1, 'Colorisation (PC)'],
['imagery_segmentation', 0, 'Watershed Segmentation'],
['imagery_segmentation', 1, 'Grid Skeletonization'],
['imagery_segmentation', 2, 'Seed Generation'],
['imagery_segmentation', 3, 'Seeded Region Growing'],
['imagery_svm', 0, 'SVM Classification'],
['imagery_tools', 0, 'Vegetation Index (Distance Based)'],
['imagery_tools', 1, 'Vegetation Index (Slope Based)'],
['imagery_tools', 2, 'Enhanced Vegetation Index'],
['imagery_tools', 3, 'Tasseled Cap Transformation'],
['imagery_tools', 4, 'IHS Sharpening'],
['imagery_tools', 5, 'Colour Normalized Brovey Sharpening'],
['imagery_tools', 6, 'Colour Normalized Spectral Sharpening'],
['imagery_tools', 7, 'Principal Component Based Image Sharpening'],
['imagery_tools', 8, 'Top of Atmosphere Reflectance'],
['imagery_tools', 9, 'Automated Cloud Cover Assessment'],
['imagery_tools', 10, 'Landsat Import with Options'],
['imagery_vigra', 0, 'Smoothing (ViGrA)'],
['imagery_vigra', 1, 'Edge Detection (ViGrA)'],
['imagery_vigra', 2, 'Morphological Filter (ViGrA)'],
['imagery_vigra', 3, 'Distance (ViGrA)'],
['imagery_vigra', 4, 'Watershed Segmentation (ViGrA)'],
['imagery_vigra', 5, 'Fourier Transform (ViGrA)'],
['imagery_vigra', 6, 'Fourier Transform Inverse (ViGrA)'],
['imagery_vigra', 7, 'Fourier Transform (Real, ViGrA)'],
['imagery_vigra', 8, 'Fourier Filter (ViGrA)'],
['imagery_vigra', 9, 'Random Forest Classification (ViGrA)'],
['imagery_vigra', 10, 'Random Forest Presence Prediction (ViGrA)'],
['io_esri_e00', 0, 'Import ESRI E00 File'],
['io_gdal', 0, 'Import Raster'],
['io_gdal', 1, 'Export Raster'],
['io_gdal', 2, 'Export GeoTIFF'],
['io_gdal', 3, 'Import Shapes'],
['io_gdal', 4, 'Export Shapes'],
['io_gdal', 5, 'Export Shapes to KML'],
['io_gdal', 6, 'Import NetCDF'],
['io_gps', 0, 'GPX to shapefile'],
['io_gps', 1, 'GPSBabel'],
['io_grid', 0, 'Export ESRI Arc/Info Grid'],
['io_grid', 1, 'Import ESRI Arc/Info Grid'],
['io_grid', 2, 'Export Surfer Grid'],
['io_grid', 3, 'Import Surfer Grid'],
['io_grid', 4, 'Import Binary Raw Data'],
['io_grid', 5, 'Export Grid to XYZ'],
['io_grid', 6, 'Import Grid from XYZ'],
['io_grid', 7, 'Import USGS SRTM Grid'],
['io_grid', 8, 'Import MOLA Grid (MEGDR)'],
['io_grid', 9, 'Import SRTM30 DEM'],
['io_grid', 10, 'Export True Color Bitmap'],
['io_grid', 11, 'Import Erdas LAN/GIS'],
['io_grid', 12, 'Import Grid from Table'],
['io_grid', 13, 'Import WRF Geogrid Binary Format'],
['io_grid', 14, 'Export WRF Geogrid Binary Format'],
['io_grid', 15, 'Import Building Sketches from CityGML'],
['io_grid_grib2', 0, 'Import GRIB2 record'],
['io_grid_image', 0, 'Export Image (bmp, jpg, pcx, png, tif)'],
['io_grid_image', 1, 'Import Image (bmp, jpg, png, tif, gif, pnm, xpm)'],
['io_grid_image', 2, 'Export Grid to KML'],
['io_shapes', 0, 'Export GStat Shapes'],
['io_shapes', 1, 'Import GStat Shapes'],
['io_shapes', 2, 'Export Shapes to XYZ'],
['io_shapes', 3, 'Import Shapes from XYZ'],
['io_shapes', 4, 'Export Shapes to Generate'],
['io_shapes', 5, 'Export Surfer Blanking File'],
['io_shapes', 6, 'Import Surfer Blanking Files'],
['io_shapes', 7, 'Export Atlas Boundary File'],
['io_shapes', 8, 'Import Atlas Boundary File'],
['io_shapes', 9, 'Export WASP terrain map file'],
['io_shapes', 10, 'Import WASP terrain map file'],
['io_shapes', 11, 'Import Stereo Lithography File (STL)'],
['io_shapes', 12, 'Export TIN to Stereo Lithography File (STL)'],
['io_shapes', 13, 'Import GPX'],
['io_shapes', 14, 'Export GPX'],
['io_shapes', 15, 'Import Point Cloud from Shape File'],
['io_shapes', 16, 'Import Point Cloud from Text File'],
['io_shapes', 17, 'Export Scalable Vector Graphics (SVG) File'],
['io_shapes', 18, 'Export Point Cloud to Text File'],
['io_shapes', 19, 'Import Simple Features from Well Known Text'],
['io_shapes', 20, 'Export Simple Features to Well Known Text'],
['io_shapes_dxf', 0, 'Import DXF Files'],
['io_shapes_las', 0, 'Export LAS Files'],
['io_shapes_las', 1, 'Import LAS Files'],
['io_shapes_las', 2, 'LAS Info'],
['io_table', 0, 'Export Text Table'],
['io_table', 1, 'Import Text Table'],
['io_table', 2, 'Import Text Table with Numbers only'],
['io_table', 3, 'Import Text Table (Fixed Column Sizes)'],
['io_virtual', 0, 'Create Virtual Point Cloud Dataset'],
['io_virtual', 1, 'Get Subset from Virtual Point Cloud'],
['io_virtual', 2, 'Create Tileshape from Virtual Point Cloud'],
['io_virtual', 4, 'Get Grid from Virtual Point Cloud'],
['io_virtual', 6, 'Remove Overlap from Virtual Point Cloud Tiles'],
['pj_georeference', 1, 'Rectify Grid'],
['pj_georeference', 2, 'Warping Shapes'],
['pj_georeference', 4, 'Direct Georeferencing of Airborne Photographs'],
['pj_georeference', 5, 'Define Georeference for Grids'],
['pj_georeference', 6, 'World File from Flight and Camera Settings'],
['pj_geotrans', 0, 'GeoTrans (Shapes)'],
['pj_geotrans', 1, 'GeoTrans (Grid)'],
['pj_proj4', 0, 'Set Coordinate Reference System'],
['pj_proj4', 1, 'Coordinate Transformation (Shapes List)'],
['pj_proj4', 2, 'Coordinate Transformation (Shapes)'],
['pj_proj4', 3, 'Coordinate Transformation (Grid List)'],
['pj_proj4', 4, 'Coordinate Transformation (Grid)'],
['pj_proj4', 5, '[deprecated] Proj.4 (Command Line Arguments, Shapes)'],
['pj_proj4', 6, '[deprecated] Proj.4 (Dialog, Shapes)'],
['pj_proj4', 7, '[deprecated] Proj.4 (Command Line Arguments, Grid)'],
['pj_proj4', 8, '[deprecated] Proj.4 (Dialog, Grid)'],
['pj_proj4', 9, '[deprecated] Proj.4 (Command Line Arguments, List of Shapes Layers)'],
['pj_proj4', 10, '[deprecated] Proj.4 (Dialog, List of Shapes Layers)'],
['pj_proj4', 11, '[deprecated] Proj.4 (Command Line Arguments, List of Grids)'],
['pj_proj4', 12, '[deprecated] Proj.4 (Dialog, List of Grids)'],
['pj_proj4', 13, 'Change Longitudinal Range for Grids'],
['pj_proj4', 14, 'Latitude/Longitude Graticule'],
['pj_proj4', 15, 'Coordinate Reference System Picker'],
['pj_proj4', 16, 'Tissot\'s Indicatrix'],
['pj_proj4', 17, 'Geographic Coordinate Grids'],
['pj_proj4', 18, 'Coordinate Transformation (Point Cloud List)'],
['pj_proj4', 19, 'Coordinate Transformation (Point Cloud)'],
['pj_proj4', 20, 'Geographic Distances'],
['pj_proj4', 21, 'Geographic Distances (Pair of Coordinates)'],
['pointcloud_tools', 0, 'Point Cloud Cutter'],
['pointcloud_tools', 2, 'Point Cloud from Grid Points'],
['pointcloud_tools', 3, 'Point Cloud from Shapes'],
['pointcloud_tools', 4, 'Point Cloud to Grid'],
['pointcloud_tools', 5, 'Point Cloud to Shapes'],
['pointcloud_tools', 6, 'Point Cloud Reclassifier / Subset Extractor'],
['pointcloud_tools', 7, 'Drop Point Cloud Attributes'],
['pointcloud_tools', 8, 'Transform Point Cloud'],
['pointcloud_tools', 9, 'Point Cloud Thinning (Simple)'],
['pointcloud_tools', 10, 'Point Cloud Attribute Calculator'],
['pointcloud_tools', 11, 'Cluster Analysis for Point Clouds'],
['pointcloud_tools', 12, 'Merge Point Clouds'],
['pointcloud_viewer', 0, '[deprecated] Point Cloud Viewer'],
['shapes_grid', 0, 'Add Grid Values to Points'],
['shapes_grid', 1, 'Add Grid Values to Shapes'],
['shapes_grid', 2, 'Grid Statistics for Polygons'],
['shapes_grid', 3, 'Grid Values to Points'],
['shapes_grid', 4, 'Grid Values to Points (randomly)'],
['shapes_grid', 5, 'Contour Lines from Grid'],
['shapes_grid', 6, 'Vectorising Grid Classes'],
['shapes_grid', 7, 'Clip Grid with Polygon'],
['shapes_grid', 8, 'Grid Statistics for Points'],
['shapes_grid', 9, 'Local Minima and Maxima'],
['shapes_grid', 10, 'Grid System Extent'],
['shapes_grid', 11, 'Clip Grid with Rectangle'],
['shapes_grid', 15, 'Gradient Vectors from Surface'],
['shapes_grid', 16, 'Gradient Vectors from Direction and Length'],
['shapes_grid', 17, 'Gradient Vectors from Directional Components'],
['shapes_lines', 0, 'Convert Polygons to Lines'],
['shapes_lines', 1, 'Convert Points to Line(s)'],
['shapes_lines', 2, 'Line Properties'],
['shapes_lines', 3, 'Line-Polygon Intersection'],
['shapes_lines', 4, 'Line Simplification'],
['shapes_lines', 5, 'Line Dissolve'],
['shapes_lines', 6, 'Split Lines with Lines'],
['shapes_lines', 7, 'Line Smoothing'],
['shapes_points', 0, 'Convert Table to Points'],
['shapes_points', 1, 'Count Points in Polygons'],
['shapes_points', 2, 'Create Point Grid'],
['shapes_points', 3, 'Point Distances'],
['shapes_points', 4, 'Populate Polygons with Points'],
['shapes_points', 5, 'Convert Lines to Points'],
['shapes_points', 6, 'Add Coordinates to Points'],
['shapes_points', 7, 'Remove Duplicate Points'],
['shapes_points', 8, 'Clip Points with Polygons'],
['shapes_points', 9, 'Separate points by direction'],
['shapes_points', 10, 'Add Polygon Attributes to Points'],
['shapes_points', 11, 'Points Filter'],
['shapes_points', 12, 'Convex Hull'],
['shapes_points', 14, 'Points Thinning'],
['shapes_points', 15, 'Convert Multipoints to Points'],
['shapes_points', 16, 'Thiessen Polygons'],
['shapes_points', 17, 'Aggregate Point Observations'],
['shapes_points', 18, 'Snap Points to Points'],
['shapes_points', 19, 'Snap Points to Lines'],
['shapes_points', 20, 'Snap Points to Grid'],
['shapes_polygons', 1, 'Polygon Centroids'],
['shapes_polygons', 2, 'Polygon Properties'],
['shapes_polygons', 3, 'Convert Lines to Polygons'],
['shapes_polygons', 4, 'Point Statistics for Polygons'],
['shapes_polygons', 5, 'Polygon Dissolve'],
['shapes_polygons', 6, 'Convert Polygon/Line Vertices to Points'],
['shapes_polygons', 7, 'Polygon Shape Indices'],
['shapes_polygons', 8, 'Polygon-Line Intersection'],
['shapes_polygons', 9, 'Polygons to Edges and Nodes'],
['shapes_polygons', 10, 'Polygon Parts to Separate Polygons'],
['shapes_polygons', 11, 'Polygon Clipping'],
['shapes_polygons', 12, 'Polygon Self-Intersection'],
['shapes_polygons', 14, 'Intersect'],
['shapes_polygons', 15, 'Difference'],
['shapes_polygons', 16, 'Symmetrical Difference'],
['shapes_polygons', 17, 'Union'],
['shapes_polygons', 18, 'Update'],
['shapes_polygons', 19, 'Identity'],
['shapes_polygons', 20, 'Add Point Attributes to Polygons'],
['shapes_polygons', 21, 'Flatten Polygon Layer'],
['shapes_polygons', 22, 'Shared Polygon Edges'],
['shapes_tools', 0, 'Create New Shapes Layer'],
['shapes_tools', 2, 'Merge Layers'],
['shapes_tools', 3, 'Select by Attributes... (Numerical Expression)'],
['shapes_tools', 4, 'Select by Attributes... (String Expression)'],
['shapes_tools', 5, 'Select by Location...'],
['shapes_tools', 6, 'Copy Selection to New Shapes Layer'],
['shapes_tools', 7, 'Delete Selection from Shapes Layer'],
['shapes_tools', 8, 'Invert Selection of Shapes Layer'],
['shapes_tools', 9, 'Split Shapes Layer Completely'],
['shapes_tools', 10, 'Transform Shapes'],
['shapes_tools', 11, 'Create Chart Layer (bars/sectors)'],
['shapes_tools', 12, 'Create Graticule'],
['shapes_tools', 13, 'Copy Shapes from Region'],
['shapes_tools', 15, 'Split Shapes Layer'],
['shapes_tools', 16, 'Split Shapes Layer Randomly'],
['shapes_tools', 17, 'Split Table/Shapes by Attribute'],
['shapes_tools', 18, 'Shapes Buffer'],
['shapes_tools', 19, 'Get Shapes Extents'],
['shapes_tools', 20, 'QuadTree Structure to Shapes'],
['shapes_tools', 21, 'Polar to Cartesian Coordinates'],
['shapes_tools', 22, 'Generate Shapes'],
['shapes_tools', 23, 'Convert Vertex Type (2D/3D)'],
['shapes_tools', 24, 'Merge Tables'],
['shapes_tools', 25, 'Land Use Scenario Generator'],
['shapes_transect', 0, 'Transect through polygon shapefile'],
['sim_cellular_automata', 0, 'Conway\'s Game of Life'],
['sim_cellular_automata', 1, 'Wa-Tor'],
['sim_ecosystems_hugget', 0, '01: A Simple Litter System'],
['sim_ecosystems_hugget', 1, '02: Carbon Cycle Simulation for Terrestrial Biomass'],
['sim_ecosystems_hugget', 2, '03: Spatially Distributed Simulation of Soil Nitrogen Dynamics'],
['sim_erosion', 0, 'MMF-SAGA Soil Erosion Model'],
['sim_fire_spreading', 0, 'Fire Risk Analysis'],
['sim_fire_spreading', 1, 'Simulation'],
['sim_hydrology', 0, 'Soil Moisture Content'],
['sim_hydrology', 1, 'Kinematic Wave Overland Flow'],
['sim_hydrology', 2, 'TOPMODEL'],
['sim_hydrology', 3, 'Water Retention Capacity'],
['sim_hydrology', 4, 'Diffuse Pollution Risk'],
['sim_ihacres', 0, 'IHACRES Calibration (2)'],
['sim_ihacres', 1, 'IHACRES Version 1.0'],
['sim_ihacres', 2, 'IHACRES Basin'],
['sim_ihacres', 3, 'IHACRES Elevation Bands'],
['sim_ihacres', 4, 'IHACRES Elevation Bands Calibration'],
['statistics_grid', 0, 'Fast Representativeness'],
['statistics_grid', 1, 'Residual Analysis (Grid)'],
['statistics_grid', 2, 'Representativeness (Grid)'],
['statistics_grid', 3, 'Radius of Variance (Grid)'],
['statistics_grid', 4, 'Statistics for Grids'],
['statistics_grid', 5, 'Zonal Grid Statistics'],
['statistics_grid', 6, 'Directional Statistics for Single Grid'],
['statistics_grid', 7, 'Global Moran\'s I for Grids'],
['statistics_grid', 8, 'Principal Component Analysis'],
['statistics_grid', 9, 'Multi-Band Variation'],
['statistics_grid', 10, 'Inverse Principal Components Rotation'],
['statistics_grid', 11, 'Longitudinal Grid Statistics'],
['statistics_grid', 12, 'Meridional Grid Statistics'],
['statistics_grid', 13, 'Save Grid Statistics to Table'],
['statistics_kriging', 0, 'Ordinary Kriging'],
['statistics_kriging', 1, 'Simple Kriging'],
['statistics_kriging', 2, 'Universal Kriging'],
['statistics_kriging', 3, 'Regression Kriging'],
['statistics_kriging', 4, 'Variogram (Dialog)'],
['statistics_points', 0, 'Variogram'],
['statistics_points', 1, 'Variogram Cloud'],
['statistics_points', 2, 'Variogram Surface'],
['statistics_points', 3, 'Minimum Distance Analysis'],
['statistics_points', 4, 'Spatial Point Pattern Analysis'],
['statistics_regression', 0, 'Regression Analysis (Points and Predictor Grid)'],
['statistics_regression', 1, 'Multiple Regression Analysis (Points and Predictor Grids)'],
['statistics_regression', 2, 'Polynomial Regression'],
['statistics_regression', 3, 'GWR for Single Predictor (Gridded Model Output)'],
['statistics_regression', 4, 'GWR for Single Predictor Grid'],
['statistics_regression', 5, 'GWR for Multiple Predictors (Gridded Model Output)'],
['statistics_regression', 6, 'GWR for Multiple Predictor Grids'],
['statistics_regression', 7, 'GWR for Multiple Predictors'],
['statistics_regression', 8, 'Multiple Regression Analysis (Grid and Predictor Grids)'],
['statistics_regression', 9, 'Polynomial Trend from Grids'],
['statistics_regression', 10, 'Trend Analysis'],
['statistics_regression', 11, 'Trend Analysis (Shapes)'],
['statistics_regression', 12, 'Multiple Linear Regression Analysis'],
['statistics_regression', 13, 'Multiple Linear Regression Analysis (Shapes)'],
['statistics_regression', 14, 'GWR for Grid Downscaling'],
['statistics_regression', 15, 'Zonal Multiple Regression Analysis (Points and Predictor Grids)'],
['ta_channels', 0, 'Channel Network'],
['ta_channels', 1, 'Watershed Basins'],
['ta_channels', 2, 'Watershed Basins (Extended)'],
['ta_channels', 3, 'Vertical Distance to Channel Network'],
['ta_channels', 4, 'Overland Flow Distance to Channel Network'],
['ta_channels', 5, 'Channel Network and Drainage Basins'],
['ta_channels', 6, 'Strahler Order'],
['ta_channels', 7, 'Valley Depth'],
['ta_compound', 0, 'Basic Terrain Analysis'],
['ta_hydrology', 0, 'Flow Accumulation (Top-Down)'],
['ta_hydrology', 1, 'Flow Accumulation (Recursive)'],
['ta_hydrology', 2, 'Flow Accumulation (Flow Tracing)'],
['ta_hydrology', 4, 'Upslope Area'],
['ta_hydrology', 6, 'Flow Path Length'],
['ta_hydrology', 7, 'Slope Length'],
['ta_hydrology', 10, 'Cell Balance'],
['ta_hydrology', 13, 'Edge Contamination'],
['ta_hydrology', 15, 'SAGA Wetness Index'],
['ta_hydrology', 16, 'Lake Flood'],
['ta_hydrology', 18, 'Flow Accumulation (Mass-Flux Method)'],
['ta_hydrology', 19, 'Flow Width and Specific Catchment Area'],
['ta_hydrology', 20, 'Topographic Wetness Index (TWI)'],
['ta_hydrology', 21, 'Stream Power Index'],
['ta_hydrology', 22, 'LS Factor'],
['ta_hydrology', 23, 'Melton Ruggedness Number'],
['ta_hydrology', 24, 'TCI Low'],
['ta_hydrology', 25, 'LS-Factor, Field Based'],
['ta_hydrology', 26, 'Slope Limited Flow Accumulation'],
['ta_lighting', 0, 'Analytical Hillshading'],
['ta_lighting', 2, 'Potential Incoming Solar Radiation'],
['ta_lighting', 3, 'Sky View Factor'],
['ta_lighting', 4, 'Topographic Correction'],
['ta_lighting', 5, 'Topographic Openness'],
['ta_lighting', 6, 'Visibility (points)'],
['ta_morphometry', 0, 'Slope, Aspect, Curvature'],
['ta_morphometry', 1, 'Convergence Index'],
['ta_morphometry', 2, 'Convergence Index (Search Radius)'],
['ta_morphometry', 3, 'Surface Specific Points'],
['ta_morphometry', 4, 'Curvature Classification'],
['ta_morphometry', 5, 'Hypsometry'],
['ta_morphometry', 6, 'Real Surface Area'],
['ta_morphometry', 7, 'Morphometric Protection Index'],
['ta_morphometry', 8, 'Multiresolution Index of Valley Bottom Flatness (MRVBF)'],
['ta_morphometry', 9, 'Downslope Distance Gradient'],
['ta_morphometry', 10, 'Mass Balance Index'],
['ta_morphometry', 11, 'Effective Air Flow Heights'],
['ta_morphometry', 12, 'Diurnal Anisotropic Heat'],
['ta_morphometry', 13, 'Land Surface Temperature'],
['ta_morphometry', 14, 'Relative Heights and Slope Positions'],
['ta_morphometry', 15, 'Wind Effect (Windward / Leeward Index)'],
['ta_morphometry', 16, 'Terrain Ruggedness Index (TRI)'],
['ta_morphometry', 17, 'Vector Ruggedness Measure (VRM)'],
['ta_morphometry', 18, 'Topographic Position Index (TPI)'],
['ta_morphometry', 19, 'TPI Based Landform Classification'],
['ta_morphometry', 20, 'Terrain Surface Texture'],
['ta_morphometry', 21, 'Terrain Surface Convexity'],
['ta_morphometry', 22, 'Terrain Surface Classification (Iwahashi and Pike)'],
['ta_morphometry', 23, 'Morphometric Features'],
['ta_morphometry', 24, 'Valley and Ridge Detection (Top Hat Approach)'],
['ta_morphometry', 25, 'Fuzzy Landform Element Classification'],
['ta_morphometry', 26, 'Upslope and Downslope Curvature'],
['ta_preprocessor', 0, 'Flat Detection'],
['ta_preprocessor', 1, 'Sink Drainage Route Detection'],
['ta_preprocessor', 2, 'Sink Removal'],
['ta_preprocessor', 3, 'Fill Sinks (Planchon/Darboux, 2001)'],
['ta_preprocessor', 4, 'Fill Sinks (Wang & Liu)'],
['ta_preprocessor', 5, 'Fill Sinks XXL (Wang & Liu)'],
['ta_preprocessor', 6, 'Burn Stream Network into DEM'],
['ta_profiles', 3, 'Cross Profiles'],
['ta_profiles', 4, 'Profiles from Lines'],
['ta_profiles', 5, 'Profile from points'],
['ta_slope_stability', 0, 'SAFETYFACTOR'],
['ta_slope_stability', 1, 'TOBIA'],
['ta_slope_stability', 2, 'SHALSTAB'],
['ta_slope_stability', 3, 'WETNESS'],
['ta_slope_stability', 4, 'WEDGEFAIL'],
['ta_slope_stability', 5, 'ANGMAP'],
['table_calculus', 0, 'Function Fit'],
['table_calculus', 1, 'Field Calculator'],
['table_calculus', 2, 'Field Calculator [Shapes]'],
['table_calculus', 5, 'Running Average'],
['table_calculus', 6, 'Cluster Analysis'],
['table_calculus', 7, 'Principal Component Analysis'],
['table_calculus', 8, 'Fill Gaps in Ordered Records'],
['table_calculus', 11, 'Find Field of Extreme Value'],
['table_calculus', 12, 'Minimum Redundancy Feature Selection'],
['table_calculus', 14, 'Cluster Analysis (Shapes)'],
['table_calculus', 15, 'Field Statistics'],
['table_tools', 0, 'Create New Table'],
['table_tools', 1, 'Transpose Table'],
['table_tools', 2, 'Table Field Enumeration'],
['table_tools', 3, 'Join Attributes from a Table'],
['table_tools', 4, 'Join Attributes from a Table (Shapes)'],
['table_tools', 5, 'Change Date Format'],
['table_tools', 6, 'Change Time Format'],
['table_tools', 7, 'Change Field Type'],
['table_tools', 8, 'Append Fields from another Table'],
['table_tools', 9, 'Change Color Format'],
['table_tools', 10, 'Replace Text'],
['table_tools', 11, 'Delete Fields'],
['table_tools', 15, 'Copy Selection'],
['table_tools', 16, 'Delete Selection'],
['table_tools', 17, 'Invert Selection'],
['table_tools', 18, 'Select by Numerical Expression'],
['table_tools', 19, 'Select by String Expression'],
['table_tools', 20, 'Add Indicator Fields for Categories'],
['tin_tools', 0, 'Grid to TIN'],
['tin_tools', 1, 'Grid to TIN (Surface Specific Points)'],
['tin_tools', 2, 'Shapes to TIN'],
['tin_tools', 3, 'TIN to Shapes'],
['tin_tools', 4, 'Gradient'],
['tin_tools', 5, 'Flow Accumulation (Trace)'],
['tin_tools', 6, 'Flow Accumulation (Parallel)'],
['tin_viewer', 0, '[deprecated] TIN Viewer'],
['sim_rivflow', 0, 'RiverBasin'],
['sim_rivflow', 1, 'LandFlow Version 1.0 (build 3.5.1b)'],
['sim_rivflow', 3, 'RiverGridGeneration'],
['sim_rivflow', 4, 'GridCombination'],
['io_grid', 16, 'Import, Clip and Resample Grids'],
['climate_tools', 8, 'ETpot (after Hargreaves, Grid)'],
['grid_visualisation', 10, 'Select Look-up Table for Grid Visualization'],
['grid_tools', 32, 'Select Grid from List'],
['imagery_maxent', 0, 'Maximum Entropy Classifcation'],
['imagery_maxent', 1, 'Maximum Entropy Presence Prediction'],
['io_grid_image', 3, 'Import Grids from KML'],
['pointcloud_tools', 13, 'Point Cloud from Table'],
['shapes_lines', 8, 'Split Lines at Points'],
['grid_filter', 16, 'Wombling (Edge Detection)'],
['grid_analysis', 21, 'Diversity of Categories'],
['climate_tools', 9, 'Sunrise and Sunset'],
['grid_filter', 17, 'Wombling for Multiple Features (Edge Detection)'],
['db_pgsql', 33, 'Import Single Raster Band from PostGIS'],
['ta_morphometry', 27, 'Wind Exposition Index'],
['imagery_classification', 6, 'Confusion Matrix (Polygons / Grid)'],
['db_pgsql', 35, 'Create Database'],
['db_pgsql', 36, 'Drop Database'],
['io_gdal', 7, 'Create Raster Catalogue from Files'],
['io_gdal', 8, 'Create Raster Catalogues from Directory'],
['sim_qm_of_esp', 0, 'Diffusive Hillslope Evolution (FTCS)'],
['sim_qm_of_esp', 1, 'Fill Sinks (QM of ESP)'],
['sim_qm_of_esp', 2, 'Flow Accumulation (QM of ESP)'],
['sim_qm_of_esp', 3, 'Successive Flow Routing'],
['sim_qm_of_esp', 4, 'Diffusive Hillslope Evolution (ADI)'],
['grid_filter', 18, 'Simple Filter (Restricted to Polygons)'],
['grid_tools', 33, 'Copy Grid'],
['grid_tools', 34, 'Invert Grid'],
['grid_tools', 35, 'Mirror Grid'],
['imagery_classification', 7, 'ISODATA Clustering for Grids'],
['io_gdal', 9, 'Import TMS Image'],
['sim_hydrology', 5, 'Surface and Gradient'],
['sim_hydrology', 6, 'Concentration'],
['sim_hydrology', 7, 'Surface, Gradient and Concentration'],
['climate_tools', 10, 'Bioclimatic Variables'],
['grid_tools', 36, 'Change a Grid\'s No-Data Value'],
['imagery_opencv', 5, 'Normal Bayes Classification (OpenCV)'],
['imagery_opencv', 6, 'K-Nearest Neighbours Classification (OpenCV)'],
['imagery_opencv', 7, 'Support Vector Machine Classification (OpenCV)'],
['imagery_opencv', 8, 'Decision Tree Classification (OpenCV)'],
['imagery_opencv', 9, 'Boosting Classification (OpenCV)'],
['imagery_opencv', 10, 'Random Forest Classification (OpenCV)'],
['imagery_opencv', 11, 'Artificial Neural Network Classification (OpenCV)'],
['io_shapes', 21, 'Import Building Sketches from CityGML'],
['statistics_grid', 14, 'Categorical Coincidence'],
['ta_hydrology', 27, 'Maximum Flow Path Length'],
['imagery_isocluster', 0, 'ISODATA Clustering for Grids'],
['shapes_lines', 9, 'Line Crossings'],
['shapes_tools', 26, 'Select Shapes from List'],
['shapes_tools', 27, 'Remove Invalid Shapes'],
['table_calculus', 16, 'Record Statistics'],
['table_calculus', 17, 'Record Statistics (Shapes)'],
['climate_tools', 11, 'Tree Growth Season'],
['imagery_tools', 11, 'Textural Features'],
['imagery_tools', 12, 'Local Statistical Measures'],
['imagery_vigra', 11, 'Random Forest Table Classification (ViGrA)'],
['io_grid', 17, 'Import CRU Grids'],
['io_shapes', 22, 'Export Polygons to HTML Image Map'],
['ta_morphometry', 28, 'Multi-Scale Topographic Position Index (TPI)'],
['climate_tools', 13, 'Wind Effect Correction'],
['shapes_grid', 18, 'Grid Classes Area for Polygons'],
['climate_tools', 14, 'Frost Change Frequency'],
['climate_tools', 15, 'Thermic Belt Classification'],
['db_pgsql', 16, 'Import Table from SQL Query (GUI)'],
['db_pgsql', 23, 'Import Shapes with Joined Data from PostGIS (GUI)'],
['grid_gridding', 8, 'Grid Cell Area Covered by Polygons'],
['grid_tools', 37, 'Combine Classes'],
['io_gdal', 10, 'GDAL Formats'],
['sim_geomorphology', 0, 'Gravitational Process Path Model'],
['statistics_grid', 15, 'Focal PCA on a Grid'],
['climate_tools', 17, 'Snow Cover'],
['pj_proj4', 23, 'UTM Projection (Grid List)'],
['pj_proj4', 24, 'UTM Projection (Grid)'],
['pj_proj4', 25, 'UTM Projection (Shapes List)'],
['pj_proj4', 26, 'UTM Projection (Shapes)'],
['pj_proj4', 27, 'UTM Projection (Point Cloud List)'],
['pj_proj4', 28, 'UTM Projection (Point Cloud)'],
['table_calculus', 18, 'Aggregate Values by Attributes'],
['climate_tools', 18, 'Growing Degree Days'],
['garden_3d_viewer', 5, 'Grid Collection Viewer'],
['grids_tools', 0, 'Create a Grid Collection'],
['grids_tools', 1, 'Extract Grids from a Grid Collection'],
['grids_tools', 2, 'Delete Grids from a Grid Collection'],
['grid_calculus', 17, 'Fractal Brownian Noise'],
['grid_tools', 38, 'Mosaicking (Grid Collections)'],
['imagery_tools', 13, 'Universal Image Quality Index'],
['imagery_tools', 14, 'Import Landsat Scene'],
['pointcloud_tools', 14, 'Select Point Cloud from List'],
['sim_landscape_evolution', 0, 'SaLEM'],
['ta_hydrology', 28, 'Flow between fields'],
['shapes_tools', 28, 'Copy Shapes'],
['table_tools', 21, 'Table Field Enumeration (Shapes)'],
['table_tools', 22, 'Copy Table'],
['db_pgsql', 34, 'Export Grid Collection to PostGIS'],
['grid_gridding', 9, 'Polygons to Grid'],
['grid_gridding', 10, 'Polygon Categories to Grid'],
['ta_lighting', 7, 'Potential Annual Insolation'],
['climate_tools', 19, 'Climate Classification'],
['climate_tools', 20, 'Soil Water Balance'],
['grid_tools', 39, 'Change Grid Values - Flood Fill'],
['io_gdal', 11, 'Import ASTER Scene'],
['shapes_tools', 29, 'Focal Mechanism (Beachball Plots)'],
['grids_tools', 3, 'Extract a Grid from a Grid Collection'],
['grids_tools', 4, 'Add a Grid to a Grid Collection'],
['grid_calculus', 20, 'Grid Collection Calculator'],
['statistics_grid', 16, 'Statistics for Grids from Files'],
['statistics_grid', 17, 'Build Statistics for Grids'],
['statistics_grid', 18, 'Evaluate Statistics for Grids'],
['table_calculus', 9, 'Fill Gaps in Records'],
['pj_proj4', 29, 'Single Coordinate Transformation'],
['shapes_points', 21, 'Create Random Points'],
['statistics_grid', 19, 'Add Statistics for Grids'],
['table_tools', 23, 'Change Field Name'],
['ta_hydrology', 29, 'Flow Accumulation (Parallelizable)'],
['table_tools', 24, 'Formatted Text'],
['table_tools', 25, 'Formatted Text [Shapes]'],
['climate_tools', 21, 'PhenIps (Table)'],
['climate_tools', 22, 'PhenIps (Grids)'],
['grids_tools', 5, 'Nearest Neighbour (3D)'],
['grids_tools', 6, 'Inverse Distance Weighted (3D)'],
['grid_calculus', 21, 'Histogram Matching'],
['grid_spline', 8, 'Multilevel B-Spline (3D)'],
['grid_visualisation', 11, 'Create a Table from Look-up Table'],
['imagery_opencv', 12, 'Logistic Regression (OpenCV)'],
['imagery_segmentation', 4, 'Superpixel Segmentation'],
['shapes_polygons', 23, 'Polygon Generalization'],
['ta_lighting', 8, 'Geomorphons']
]
