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
    public rtengine::LocallabListener,
    public ThresholdAdjusterListener

{
private: 
    IdleRegister idle_register;

    // Expander widgets
    ControlSpotPanel* const expsettings;
    MyExpander* const expcolor;
    MyExpander* const expexpose;
    MyExpander* const expvibrance;
    MyExpander* const expsoft;
    MyExpander* const explabregion;
    MyExpander* const expblur;
    MyExpander* const exptonemap;
    MyExpander* const expreti;
    MyExpander* const expsharp;
    MyExpander* const expcontrast;
    MyExpander* const expcbdl;
    MyExpander* const expdenoi;
    sigc::connection enablecolorConn, enableexposeConn, enablevibranceConn, enablesoftConn, enablelabregionConn, enableblurConn, enabletonemapConn, enableretiConn, enablesharpConn, enablecontrastConn, enablecbdlConn, enabledenoiConn;

    // Curve widgets
    // Color & Light
    CurveEditorGroup* const llCurveEditorG;
    CurveEditorGroup* const maskCurveEditorG;
    DiagonalCurveEditor* llshape;
    DiagonalCurveEditor* ccshape;
    FlatCurveEditor* LHshape;
    FlatCurveEditor* HHshape;
    FlatCurveEditor* CCmaskshape;
    FlatCurveEditor* LLmaskshape;
    FlatCurveEditor* HHmaskshape;
    
    // Exposure
    CurveEditorGroup* const curveEditorG;
    CurveEditorGroup* const maskexpCurveEditorG;
    DiagonalCurveEditor* shapeexpos;
    FlatCurveEditor* CCmaskexpshape;
    FlatCurveEditor* LLmaskexpshape;
    FlatCurveEditor* HHmaskexpshape;
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
    Adjuster* const structcol;
    Adjuster* const blendmaskcol;

    // Exposure
    Adjuster* const expcomp;
    Adjuster* const hlcompr;
    Adjuster* const hlcomprthresh;
    Adjuster* const black;
    Adjuster* const shcompr;
    Adjuster* const warm;
    Adjuster* const sensiex;
    Adjuster* const structexp;
    // Vibrance
    Adjuster* const saturated;
    Adjuster* const pastels;
    Adjuster* const sensiv;
    // Soft Light
    Adjuster* const streng;
    Adjuster* const sensisf;
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
    Adjuster* const dehaz;    
    Adjuster* const sensih;
    // Sharpening
    Adjuster* const sharcontrast;
    Adjuster* const sharradius;
    Adjuster* const sharamount;
    Adjuster* const shardamping;
    Adjuster* const shariter;
    Adjuster* const sharblur;
    Adjuster* const sensisha;
    // Local Contrast
    Adjuster* const lcradius;
    Adjuster* const lcamount;
    Adjuster* const lcdarkness;
    Adjuster* const lclightness;
    Adjuster* const sensilc;
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
    MyComboBoxText* const showmaskcolMethod;
    sigc::connection showmaskcolMethodConn;
    //Exposure
    MyComboBoxText* const showmaskexpMethod;
    sigc::connection showmaskexpMethodConn;

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
    Gtk::Label* transLabels;
    Gtk::Label* transLabels2;

    //Frame
    Gtk::Frame* maskcolFrame;
    Gtk::Frame* maskexpFrame;
    
    // Others
    /**
     * Used to store the default ProcParams when setDefaults function is called
     * When an other spot is selected, this default ProcParams is used to update adjusters default values
     */
    const rtengine::ProcParams* defparams;
    /**
     * Used to store the default ParamsEdited when setDefaults function is called
     * When an other spot is selected, this default ParamsEdited is used to update adjusters default edited state
     */
    const ParamsEdited* defpedited;
    /**
     * Used to store the default ParamsEdited when setDefaults function is called
     * This ParamsEdited is updated when control spots are modified and is used to update adjusters edited state
     */
    ParamsEdited* pe;

    // Expander management functions
    void foldAllButMe(GdkEventButton* event, MyExpander *expander);
    void enableToggled(MyExpander *expander);

    // ButtonCheck event functions
    // Color & Light
    void curvactivChanged();
    void inversChanged();
    void showmaskcolMethodChanged();
    //Exposure
    void showmaskexpMethodChanged();

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
    void updateLocallabGUI(const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited, int index);
    void updateSpecificGUIState();
    void setParamEditable(bool cond);
    double nexthuer;
    double nextlumar;
    double nextchromar;

/*
    void onLabRegionSelectionChanged();
    void labRegionAddPressed();
    void labRegionRemovePressed();
    void labRegionUpPressed();
    void labRegionDownPressed();
    void labRegionCopyPressed();
    void labRegionShowMaskChanged();
    void labRegionChannelChanged();
    void labRegionPopulateList();
    void labRegionShow(int idx, bool list_only=false);
    void labRegionGet(int idx);

    Gtk::VBox *labRegionBox;
    Gtk::ListViewText *labRegionList;
    Gtk::Button *labRegionAdd;
    Gtk::Button *labRegionRemove;
    Gtk::Button *labRegionUp;
    Gtk::Button *labRegionDown;
    Gtk::Button *labRegionCopy;
    LabGrid *labRegionAB;
    Adjuster *labRegionSaturation;
    */
 //   Adjuster *labRegionSlope;
    /*
    Adjuster *labRegionOffset;
    Adjuster *labRegionPower;
    MyComboBoxText *labRegionChannel;
    FlatCurveEditor *labRegionHueMask;
    FlatCurveEditor *labRegionChromaticityMask;
    FlatCurveEditor *labRegionLightnessMask;
    Adjuster *labRegionMaskBlur;
    Gtk::CheckButton *labRegionShowMask;
    std::vector<rtengine::ColorToningParams::LabCorrectionRegion> labRegionData;
    int labRegionSelected;
    sigc::connection labRegionSelectionConn;
*/

public:
    Locallab();
    ~Locallab();

    // FoldableToolPanel management functions
    void read(const rtengine::procparams::ProcParams* pp, const ParamsEdited* pedited = nullptr);
    void write(rtengine::procparams::ProcParams* pp, ParamsEdited* pedited = nullptr);
    void setDefaults(const rtengine::procparams::ProcParams* defParams, const ParamsEdited* pedited = nullptr);
    void setDefaults(const rtengine::procparams::ProcParams* defParams, const ParamsEdited* pedited, int id);
    void setBatchMode(bool batchMode);
    void trimValues(rtengine::procparams::ProcParams* pp);
    void setListener(ToolPanelListener* tpl);
    void enableListener();
    void disableListener();
    void writeOptions(std::vector<int> &tpOpen);
    void updateToolState(std::vector<int> &tpOpen);
    void refChanged (double huer, double lumar, double chromar);
    bool refComputed_ ();
    void updateLabel      ();

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