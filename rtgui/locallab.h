/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 *  2017 Jacques Desmis <jdesmis@gmail.com>
 *  2018 Pierre Cabrera <pierre.cab@gmail.com>
 */

#include <gtkmm.h>
#include "adjuster.h"
#include "toolpanel.h"
#include "edit.h"
#include "curveeditor.h"
#include "curveeditorgroup.h"
#include "toolpanel.h"
#include "options.h"
#include "thresholdadjuster.h"
#include "controlspotpanel.h"

class Locallab :
    public ToolParamBlock,
    public AdjusterListener,
    public FoldableToolPanel,
    public CurveListener,
    public ColorProvider,
    public ThresholdCurveProvider,
    public ThresholdAdjusterListener

{
private:
    // Expander widgets
    ControlSpotPanel* const expsettings;
    MyExpander* const expcolor;
    MyExpander* const expexpose;
    MyExpander* const expvibrance;
    MyExpander* const expblur;
    MyExpander* const exptonemap;
    MyExpander* const expreti;
    MyExpander* const expsharp;
    MyExpander* const expcbdl;
    MyExpander* const expdenoi;
    sigc::connection enablecolorConn, enableexposeConn, enablevibranceConn, enableblurConn, enabletonemapConn, enableretiConn, enablesharpConn, enablecbdlConn, enabledenoiConn;

    // Curve widgets
    // Color & Light
    CurveEditorGroup* const llCurveEditorG;
    DiagonalCurveEditor* llshape;
    DiagonalCurveEditor* ccshape;
    FlatCurveEditor* LHshape;
    FlatCurveEditor* HHshape;
    // Exposure
    CurveEditorGroup* const curveEditorG;
    DiagonalCurveEditor* shapeexpos;
    // Vibrance
    CurveEditorGroup* const curveEditorGG;
    DiagonalCurveEditor* skinTonesCurve;
    // Retinex
    CurveEditorGroup* const LocalcurveEditorgainT;
    FlatCurveEditor* cTgainshape;

    // Adjuster widgets
    // Color & Light
    Adjuster* const lightness;
    Adjuster* const contrast;
    Adjuster* const chroma;
    Adjuster* const sensi;
    // Exposure
    Adjuster* const expcomp;
    Adjuster* const hlcompr;
    Adjuster* const hlcomprthresh;
    Adjuster* const black;
    Adjuster* const shcompr;
    Adjuster* const warm;
    Adjuster* const sensiex;
    // Vibrance
    Adjuster* const saturated;
    Adjuster* const pastels;
    Adjuster* const sensiv;
    // Blur & Noise
    Adjuster* const radius;
    Adjuster* const strength;
    Adjuster* const sensibn;
    // Tone Mapping
    Adjuster* const stren;
    Adjuster* const gamma;
    Adjuster* const estop;
    Adjuster* const scaltm;
    Adjuster* const rewei;
    Adjuster* const sensitm;
    // Retinex
    Adjuster* const str;
    Adjuster* const chrrt;
    Adjuster* const neigh;
    Adjuster* const vart;
    Adjuster* const sensih;
    // Sharpening
    Adjuster* const sharradius;
    Adjuster* const sharamount;
    Adjuster* const shardamping;
    Adjuster* const shariter;
    Adjuster* const sensisha;
    // Contrast by detail levels
    Adjuster* multiplier[5];
    Adjuster* const chromacbdl;
    Adjuster* const threshold;
    Adjuster* const sensicb;
    // Denoise
    Adjuster* const noiselumf;
    Adjuster* const noiselumc;
    Adjuster* const noiselumdetail;
    Adjuster* const noiselequal;
    Adjuster* const noisechrof;
    Adjuster* const noisechroc;
    Adjuster* const noisechrodetail;
    Adjuster* const adjblur;
    Adjuster* const bilateral;
    Adjuster* const sensiden;

    // ButtonCheck widgets
    // Color & Light
    Gtk::CheckButton* const curvactiv;
    Gtk::CheckButton* const invers;
    sigc::connection curvactivConn, inversConn;
    // Vibrance
    Gtk::CheckButton* const protectSkins;
    Gtk::CheckButton* const avoidColorShift;
    Gtk::CheckButton* const pastSatTog;
    sigc::connection pskinsconn, ashiftconn, pastsattogconn;
    // Blur & Noise
    Gtk::CheckButton* const activlum;
    sigc::connection activlumConn;
    // Retinex
    Gtk::CheckButton* const inversret;
    sigc::connection inversretConn;
    // Sharpening
    Gtk::CheckButton* const inverssha;
    sigc::connection inversshaConn;
    // Others
    Gtk::CheckButton* const avoid;
    sigc::connection avoidConn;

    // ComboBox widgets
    // Color & Light
    MyComboBoxText* const qualitycurveMethod;
    sigc::connection qualitycurveMethodConn;
    // Blur & Noise
    MyComboBoxText* const blurMethod;
    sigc::connection blurMethodConn;
    // Retinex
    MyComboBoxText* const retinexMethod;
    sigc::connection retinexMethodConn;

    // ThresholdAdjuster widgets
    // Vibrance
    ThresholdAdjuster* const psThreshold;

    // Other widgets
    Gtk::Label* const labqualcurv;
    Gtk::Button* const lumacontrastMinusButton;
    Gtk::Button* const lumaneutralButton;
    Gtk::Button* const lumacontrastPlusButton;
    sigc::connection lumacontrastMinusPressedConn, lumaneutralPressedConn, lumacontrastPlusPressedConn;

    // Expander management functions
    void foldAllButMe(GdkEventButton* event, MyExpander *expander);
    void enableToggled(MyExpander *expander);

    // ButtonCheck event functions
    // Color & Light
    void curvactivChanged();
    void inversChanged();
    // Vibrance
    void protectskins_toggled();
    void avoidcolorshift_toggled();
    void pastsattog_toggled();
    // Blur & Noise
    void activlumChanged();
    // Retinex
    void inversretChanged();
    // Sharpening
    void inversshaChanged();
    // Others
    void avoidChanged();

    // ComboBox event functions
    // Color & Light
    void qualitycurveMethodChanged();
    // Blur & Noise
    void blurMethodChanged();
    // Retinex
    void retinexMethodChanged();

    // Other widgets event functions
    void lumacontrastMinusPressed();
    void lumaneutralPressed();
    void lumacontrastPlusPressed();

    // Locallab GUI management function
    void updateLocallabGUI(const rtengine::procparams::ProcParams* pp, int index);
    void updateSpecificGUIState();

public:
    Locallab();
    ~Locallab();

    // FoldableToolPanel management functions
    void read(const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited = nullptr);
    void write(rtengine::procparams::ProcParams* pp, ParamsEdited* pedited = nullptr);
    void setDefaults(const rtengine::procparams::ProcParams* defParams, const ParamsEdited* pedited = nullptr);
    void setBatchMode(bool batchMode);
    void trimValues(rtengine::procparams::ProcParams* pp);
    void setListener(ToolPanelListener* tpl);
    void enableListener();
    void disableListener();
    void writeOptions(std::vector<int> &tpOpen);
    void updateToolState(std::vector<int> &tpOpen);

    // EditProvider management function
    void setEditProvider(EditDataProvider* provider);
    void subscribe();
    void unsubscribe();

    // FoldableToolPanel event function
    void enabledChanged();

    // Curve management function
    void autoOpenCurve();

    // Curve event function
    void curveChanged(CurveEditor* ce);

    // Adjuster event function
    void adjusterChanged(Adjuster* a, double newval);
    void adjusterAutoToggled(Adjuster* a, bool newval);

    // ThresholdAdjuster event functions
    virtual void colorForValue(double valX, double valY, enum ColorCaller::ElemType elemType, int callerId, ColorCaller* caller);
    std::vector<double> getCurvePoints(ThresholdSelector* tAdjuster) const;
    void adjusterChanged(ThresholdAdjuster* a, int newBottom, int newTop);
    void adjusterChanged(ThresholdAdjuster* a, double newBottom, double newTop);
    void adjusterChanged(ThresholdAdjuster* a, double newBottomLeft, double newTopLeft, double newBottomRight, double newTopRight);
    void adjusterChanged(ThresholdAdjuster* a, int newBottomLeft, int newTopLeft, int newBottomRight, int newTopRight);
    void adjusterChanged2(ThresholdAdjuster* a, int newBottomL, int newTopL, int newBottomR, int newTopR);


};