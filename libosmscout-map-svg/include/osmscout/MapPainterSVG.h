#ifndef OSMSCOUT_MAP_MAPPAINTERSVG_H
#define OSMSCOUT_MAP_MAPPAINTERSVG_H

/*
  This source is part of the libosmscout-map library
  Copyright (C) 2011  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <ostream>
#include <map>
#include <mutex>
#include <unordered_map>
#include <set>

#include <osmscout/MapSVGFeatures.h>

#if defined(OSMSCOUT_MAP_SVG_HAVE_LIB_PANGO)
  #include <pango/pangoft2.h>
#endif

#include <osmscout/MapSVGImportExport.h>

#include <osmscout/MapPainter.h>

namespace osmscout {

  class OSMSCOUT_MAP_SVG_API MapPainterSVG : public MapPainter
  {
  private:
#if defined(OSMSCOUT_MAP_SVG_HAVE_LIB_PANGO)
    typedef std::unordered_map<size_t,PangoFontDescription*>  FontMap;          //! Map type for mapping  font sizes to font

    PangoFontMap                     *pangoFontMap;
    PangoContext                     *pangoContext;
    FontMap                          fonts;            //! Cached scaled font
#endif
     std::map<FillStyle,std::string> fillStyleNameMap;
     std::map<LineStyle,std::string> lineStyleNameMap;
     std::ostream                    stream;
     TypeConfigRef                   typeConfig;
    std::mutex                       mutex;         //! Mutex for locking concurrent calls

  private:
    std::string GetColorValue(const Color& color);

#if defined(OSMSCOUT_MAP_SVG_HAVE_LIB_PANGO)
    PangoFontDescription* GetFont(const Projection& projection,
                                  const MapParameter& parameter,
                                  double fontSize);
#endif

    void SetupFillAndStroke(const FillStyleRef &fillStyle,
                                           const BorderStyleRef &borderStyle);

    void WriteHeader(size_t width,size_t height);
    void DumpStyles(const StyleConfig& styleConfig,
                    const MapParameter& parameter,
                    const Projection& projection);
    void WriteFooter();

    void StartMainGroup();
    void FinishMainGroup();

  protected:
    void AfterPreprocessing(const StyleConfig& styleConfig,
                            const Projection& projection,
                            const MapParameter& parameter,
                            const MapData& data) override;

    void BeforeDrawing(const StyleConfig& styleConfig,
                       const Projection& projection,
                       const MapParameter& parameter,
                       const MapData& data) override;

    void AfterDrawing(const StyleConfig& styleConfig,
                      const Projection& projection,
                      const MapParameter& parameter,
                      const MapData& data) override;

    bool HasIcon(const StyleConfig& styleConfig,
                 const MapParameter& parameter,
                 IconStyle& style) override;

    double GetFontHeight(const Projection& projection,
                       const MapParameter& parameter,
                       double fontSize) override;

    TextDimension GetTextDimension(const Projection& projection,
                                   const MapParameter& parameter,
                                   double objectWidth,
                                   double fontSize,
                                   const std::string& text) override;

    void DrawGround(const Projection& projection,
                    const MapParameter& parameter,
                    const FillStyle& style) override;

    void DrawLabel(const Projection& projection,
                   const MapParameter& parameter,
                   const LabelData& label) override;

    void DrawSymbol(const Projection& projection,
                    const MapParameter& parameter,
                    const Symbol& style,
                    double x, double y) override;

    void DrawIcon(const IconStyle* style,
                  double x, double y) override;

    void DrawPath(const Projection& projection,
                  const MapParameter& parameter,
                  const Color& color,
                  double width,
                  const std::vector<double>& dash,
                  LineStyle::CapStyle startCap,
                  LineStyle::CapStyle endCap,
                  size_t transStart, size_t transEnd) override;

    void DrawPath(const Projection& projection,
                  const MapParameter& parameter,
                  const std::string& styleName,
                  double width,
                  LineStyle::CapStyle startCap,
                  LineStyle::CapStyle endCap,
                  size_t transStart, size_t transEnd);

    void DrawWayOutline(const StyleConfig& styleConfig,
                        const Projection& projection,
                        const MapParameter& parameter,
                        const WayData& data);

    void DrawWay(const StyleConfig& styleConfig,
                 const Projection& projection,
                 const MapParameter& parameter,
                 const WayData& data) override;

    void DrawContourLabel(const Projection& projection,
                          const MapParameter& parameter,
                          const PathTextStyle& style,
                          const std::string& text,
                          size_t transStart, size_t transEnd,
                          ContourLabelHelper& helper) override;

    void DrawContourSymbol(const Projection& projection,
                           const MapParameter& parameter,
                           const Symbol& symbol,
                           double space,
                           size_t transStart, size_t transEnd) override;

    void DrawArea(const Projection& projection,
                  const MapParameter& parameter,
                  const AreaData& area) override;

  public:
    explicit MapPainterSVG(const StyleConfigRef& styleConfig);
    ~MapPainterSVG() override;


    bool DrawMap(const Projection& projection,
                 const MapParameter& parameter,
                 const MapData& data,
                 std::ostream& stream);
  };
}

#endif
