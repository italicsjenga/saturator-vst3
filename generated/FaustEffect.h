/* ------------------------------------------------------------
name: "FaustEffect"
Code generated with Faust 2.40.0 (https://faust.grame.fr)
Compilation options: -a C:/Users/janka/Documents/Projects/Programming/JuceVSTs/clipping\Faust\faustMinimal.h -lang cpp -i -scn faustdsp -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h ******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***********************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
    
    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string replaceCharList(std::string str, const std::vector<char>& ch1, char ch2)
        {
            std::vector<char>::const_iterator beg = ch1.begin();
            std::vector<char>::const_iterator end = ch1.end();
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) {
                    str[i] = ch2;
                }
            }
            return str;
        }
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::vector<char> rep = {' ', '#', '*', ',', '/', '?', '[', ']', '{', '}', '(', ')'};
            replaceCharList(res, rep, '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of complete hierarchical 'paths' and zones for each UI item.
 *
 * Simple 'labels' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // setParamValue/getParamValue
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            } else {
                fprintf(stderr, "ERROR : setParamValue '%s' not found\n", path.c_str());
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                fprintf(stderr, "ERROR : getParamValue '%s' not found\n", path.c_str());
                return 0;
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamAddress1(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            } else if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in faustdsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct Meta
{
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN faustdsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t count) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param size - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(size_t size, size_t reads, size_t writes) {}
    
    /**
     * Inform the Memory Manager that all memory zones have been described,
     * to possibly start a 'compute the best allocation strategy' step.
     */
    virtual void end() {}
    
    /**
     * Allocate a memory zone.
     * @param size - the memory zone size in bytes
     */
    virtual void* allocate(size_t size) = 0;
    
    /**
     * Destroy a memory zone.
     * @param ptr - the memory zone pointer to be deallocated
     */
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class faustdsp {

    public:

        faustdsp() {}
        virtual ~faustdsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual faustdsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public faustdsp {

    protected:

        faustdsp* fDSP;

    public:

        decorator_dsp(faustdsp* faustdsp = nullptr):fDSP(faustdsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual faustdsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class ScopedNoDenormals
{
    private:
    
        intptr_t fpsr;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
           asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined ( __SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #else
            #if defined(__SSE__)
            #if defined(__SSE2__)
                intptr_t mask = 0x8040;
            #else
                intptr_t mask = 0x8000;
            #endif
            #else
                intptr_t mask = 0x0000;
            #endif
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals();

#endif

/************************** END faustdsp.h **************************/
/************************** BEGIN one-sample-faustdsp.h ************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __one_sample_dsp__
#define __one_sample_dsp__

#include <assert.h>

class one_sample_dsp : public faustdsp {
  
    protected:
        
        FAUSTFLOAT* fInputs;
        FAUSTFLOAT* fOutputs;
    
        int* iControl;
        FAUSTFLOAT* fControl;
    
        bool fDelete;
    
        void checkAlloc()
        {
            // Allocated once (TODO : make this RT safe)
            if (!fInputs) {
                fInputs = new FAUSTFLOAT[getNumInputs() * 4096];
                fOutputs = new FAUSTFLOAT[getNumOutputs() * 4096];
            }
            if (!iControl) {
                iControl = new int[getNumIntControls()];
                fControl = new FAUSTFLOAT[getNumRealControls()];
            }
        }
    
    public:
    
        one_sample_dsp():fInputs(nullptr), fOutputs(nullptr), iControl(nullptr), fControl(nullptr), fDelete(true)
        {}
        
        one_sample_dsp(int* iControl, FAUSTFLOAT* fControl)
        :fInputs(nullptr), fOutputs(nullptr),
        iControl(iControl), fControl(fControl), fDelete(false)
        {}
        
        virtual ~one_sample_dsp()
        {
            delete [] fInputs;
            delete [] fOutputs;
            if (fDelete) {
                delete [] iControl;
                delete [] fControl;
            }
        }
    
        /**
         * Return the number of 'int' typed values necessary to compute the internal DSP state
         *
         * @return the number of 'int' typed values.
         */
        virtual int getNumIntControls() = 0;
    
        /**
         * Return the number of 'float, double or quad' typed values necessary to compute the DSP control state
         *
         * @return the number of 'float, double or quad' typed values.
         */
        virtual int getNumRealControls() = 0;
    
        /**
         * Update the DSP control state.
         *
         * @param iControl - an externally allocated array of 'int' typed values used to keep the DSP control state
         * @param fControl - an externally allocated array of 'float, double or quad' typed values used to keep the DSP control state
         */
        virtual void control(int* iControl, FAUSTFLOAT* fControl) = 0;
    
        // Alternative external version
        virtual void control()
        {
            checkAlloc();
            control(iControl, fControl);
        }
        
        /**
         * Compute one sample.
         *
         * @param inputs - the input audio buffers as an array of getNumInputs FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of getNumOutputs FAUSTFLOAT samples (either float, double or quad)
         * @param iControl - the externally allocated array of 'int' typed values used to keep the DSP control state
         * @param fControl - the externally allocated array of 'float, double or quad' typed values used to keep the DSP control state
         */
        virtual void compute(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs, int* iControl, FAUSTFLOAT* fControl) = 0;
    
        // The standard 'compute' expressed using the control/compute (one sample) model
        virtual void compute(int count, FAUSTFLOAT** inputs_aux, FAUSTFLOAT** outputs_aux)
        {
            // Control
            control();
            
            // Compute
            int num_inputs = getNumInputs();
            int num_outputs = getNumOutputs();
            
            FAUSTFLOAT* inputs_ptr = &fInputs[0];
            FAUSTFLOAT* outputs_ptr = &fOutputs[0];
            
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_inputs; chan++) {
                    inputs_ptr[chan] = inputs_aux[chan][frame];
                }
                inputs_ptr += num_inputs;
            }
            
            inputs_ptr = &fInputs[0];
            for (int frame = 0; frame < count; frame++) {
                // One sample compute
                compute(inputs_ptr, outputs_ptr, iControl, fControl);
                inputs_ptr += num_inputs;
                outputs_ptr += num_outputs;
            }
            
            outputs_ptr = &fOutputs[0];
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_outputs; chan++) {
                    outputs_aux[chan][frame] = outputs_ptr[chan];
                }
                outputs_ptr += num_outputs;
            }
        }
        
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
    
        int* getIControl() { return iControl; }
        FAUSTFLOAT* getFControl() { return fControl; }
    
};

// To be used with -os1 and -os2 mode

template <typename REAL>
class one_sample_dsp_real : public faustdsp {
    
    protected:
    
        FAUSTFLOAT* fInputs;
        FAUSTFLOAT* fOutputs;
        
        int* iControl;
        FAUSTFLOAT* fControl;
    
        int* iZone;
        REAL* fZone;
    
        bool fDelete;
    
        void checkAlloc()
        {
            // Allocated once (TODO : make this RT safe)
            if (!fInputs) {
                fInputs = new FAUSTFLOAT[getNumInputs() * 4096];
                fOutputs = new FAUSTFLOAT[getNumOutputs() * 4096];
            }
            if (!iControl) {
                iControl = new int[getNumIntControls()];
                fControl = new FAUSTFLOAT[getNumRealControls()];
                iZone = new int[getiZoneSize()];
                fZone = new REAL[getfZoneSize()];
            }
        }
    
    public:
    
        one_sample_dsp_real()
        :fInputs(nullptr), fOutputs(nullptr),
        iControl(nullptr), fControl(nullptr),
        iZone(nullptr), fZone(nullptr), fDelete(true)
        {}
    
        one_sample_dsp_real(int* iControl, FAUSTFLOAT* fControl, int* iZone, REAL* fZone)
        :fInputs(nullptr), fOutputs(nullptr),
        iControl(iControl), fControl(fControl),
        iZone(iZone), fZone(fZone), fDelete(false)
        {}
        
        virtual ~one_sample_dsp_real()
        {
            delete [] fInputs;
            delete [] fOutputs;
            if (fDelete) {
                delete [] iControl;
                delete [] fControl;
                delete [] iZone;
                delete [] fZone;
            }
        }
    
        virtual void init(int sample_rate)
        {
            checkAlloc();
            init(sample_rate, iZone, fZone);
        }
    
        virtual void init(int sample_rate, int* iZone, REAL* fZone) = 0;
    
        virtual void instanceInit(int sample_rate)
        {
            checkAlloc();
            instanceInit(sample_rate, iZone, fZone);
        }
    
        virtual void instanceInit(int sample_rate, int* iZone, REAL* fZone) = 0;
 
        virtual void instanceConstants(int sample_rate)
        {
            checkAlloc();
            instanceConstants(sample_rate, iZone, fZone);
        }
    
        virtual void instanceConstants(int sample_rate, int* iZone, REAL* fZone) = 0;
  
        virtual void instanceClear()
        {
            checkAlloc();
            instanceClear(iZone, fZone);
        }
    
        virtual void instanceClear(int* iZone, REAL* fZone) = 0;

        /**
         * Return the number of 'int' typed values necessary to compute the internal DSP state
         *
         * @return the number of 'int' typed values.
         */
        virtual int getNumIntControls() = 0;
        
        /**
         * Return the number of 'float, double or quad' typed values necessary to compute the DSP control state
         *
         * @return the number of 'float, double or quad' typed values.
         */
        virtual int getNumRealControls() = 0;
    
        /**
        * Return the size on 'float, double or quad' typed values necessary to compute the DSP state
        *
        * @return the number of 'float, double or quad' typed values.
        */
        virtual int getiZoneSize() = 0;
        
        /**
         * Return the size on 'int' typed values necessary to compute the DSP state
         *
         * @return the number of 'int' typed values.
         */
        virtual int getfZoneSize() = 0;
    
        /**
         * Update the DSP control state.
         *
         * @param iControl - an externally allocated array of 'int' typed values used to keep the DSP control state
         * @param fControl - an externally allocated array of 'float, double or quad' typed values used to keep the DSP control state
         * @param iZone - an externally allocated array of 'int' typed values used to keep the DSP state
         * @param fZone - an externally allocated array of 'float, double or quad' typed values used to keep the DSP state
         */
        virtual void control(int* iControl, FAUSTFLOAT* fControl, int* iZone, REAL* fZone) = 0;
        
        // Alternative external version
        virtual void control()
        {
            control(iControl, fControl, iZone, fZone);
        }
    
        /**
         * Compute one sample.
         *
         * @param inputs - the input audio buffers as an array of getNumInputs FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of getNumOutputs FAUSTFLOAT samples (either float, double or quad)
         * @param iControl - the externally allocated array of 'int' typed values used to keep the DSP control state
         * @param fControl - the externally allocated array of 'float, double or quad' typed values used to keep the DSP control state
         * @param iZone - an externally allocated array of 'int' typed values used to keep the DSP state
         * @param fZone - an externally allocated array of 'float, double or quad' typed values used to keep the DSP state
         */
        virtual void compute(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs,
                             int* iControl, FAUSTFLOAT* fControl,
                             int* iZone, REAL* fZone) = 0;
        
        // The standard 'compute' expressed using the control/compute (one sample) model
        virtual void compute(int count, FAUSTFLOAT** inputs_aux, FAUSTFLOAT** outputs_aux)
        {
            assert(fInputs);
            
            // Control
            control();
            
            // Compute
            int num_inputs = getNumInputs();
            int num_outputs = getNumOutputs();
            
            FAUSTFLOAT* inputs_ptr = &fInputs[0];
            FAUSTFLOAT* outputs_ptr = &fOutputs[0];
            
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_inputs; chan++) {
                    inputs_ptr[chan] = inputs_aux[chan][frame];
                }
                inputs_ptr += num_inputs;
            }
            
            inputs_ptr = &fInputs[0];
            for (int frame = 0; frame < count; frame++) {
                // One sample compute
                compute(inputs_ptr, outputs_ptr, iControl, fControl, iZone, fZone);
                inputs_ptr += num_inputs;
                outputs_ptr += num_outputs;
            }
            
            outputs_ptr = &fOutputs[0];
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_outputs; chan++) {
                    outputs_aux[chan][frame] = outputs_ptr[chan];
                }
                outputs_ptr += num_outputs;
            }
        }
        
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
    
        int* getIControl() { return iControl; }
        FAUSTFLOAT* getFControl() { return fControl; }
        
        int* getIZone() { return iZone; }
        REAL* getFZone() { return fZone; }
    
};

// To be used with -os3 mode

template <typename REAL>
class one_sample_dsp_real1 : public faustdsp {
    
    protected:
        
        FAUSTFLOAT* fInputs;
        FAUSTFLOAT* fOutputs;
            
    public:
        
        one_sample_dsp_real1():fInputs(nullptr), fOutputs(nullptr)
        {}
        
        virtual ~one_sample_dsp_real1()
        {
            delete [] fInputs;
            delete [] fOutputs;
        }
        
        /**
         * Return the number of 'int' typed values necessary to compute the internal DSP state
         *
         * @return the number of 'int' typed values.
         */
        virtual int getNumIntControls() = 0;
        
        /**
         * Return the number of 'float, double or quad' typed values necessary to compute the DSP control state
         *
         * @return the number of 'float, double or quad' typed values.
         */
        virtual int getNumRealControls() = 0;
        
        /**
         * Return the size on 'float, double or quad' typed values necessary to compute the DSP state
         *
         * @return the number of 'float, double or quad' typed values.
         */
        virtual int getiZoneSize() = 0;
        
        /**
         * Return the size on 'int' typed values necessary to compute the DSP state
         *
         * @return the number of 'int' typed values.
         */
        virtual int getfZoneSize() = 0;
        
        /**
         * Update the DSP control state.
         */
        virtual void control() = 0;
        
        /**
         * Compute one sample.
         *
         * @param inputs - the input audio buffers as an array of getNumInputs FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of getNumOutputs FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) = 0;
        
        // The standard 'compute' expressed using the control/compute (one sample) model
        virtual void compute(int count, FAUSTFLOAT** inputs_aux, FAUSTFLOAT** outputs_aux)
        {
            // TODO : not RT safe
            if (!fInputs) {
                fInputs = new FAUSTFLOAT[getNumInputs() * 4096];
                fOutputs = new FAUSTFLOAT[getNumOutputs() * 4096];
            }
            
            // Control
            control();
            
            // Compute
            int num_inputs = getNumInputs();
            int num_outputs = getNumOutputs();
            
            FAUSTFLOAT* inputs_ptr = &fInputs[0];
            FAUSTFLOAT* outputs_ptr = &fOutputs[0];
            
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_inputs; chan++) {
                    inputs_ptr[chan] = inputs_aux[chan][frame];
                }
                inputs_ptr += num_inputs;
            }
            
            inputs_ptr = &fInputs[0];
            for (int frame = 0; frame < count; frame++) {
                // One sample compute
                compute(inputs_ptr, outputs_ptr);
                inputs_ptr += num_inputs;
                outputs_ptr += num_outputs;
            }
            
            outputs_ptr = &fOutputs[0];
            for (int frame = 0; frame < count; frame++) {
                for (int chan = 0; chan < num_outputs; chan++) {
                    outputs_aux[chan][frame] = outputs_ptr[chan];
                }
                outputs_ptr += num_outputs;
            }
        }
        
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
  
};

#endif
/************************** END one-sample-faustdsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float mydsp_faustpower2_f(float value) {
	return value * value;
}

class mydsp : public faustdsp {
	
 private:
	
	FAUSTFLOAT fEntry0;
	float fVec0[2];
	int fSampleRate;
	float fConst1;
	FAUSTFLOAT fEntry1;
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fEntry2;
	float fRec1[2];
	float fVec1[2];
	float fConst3;
	float fVec2[3];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA1:license", "LGPL v3.0 license");
		m->declare("aanl.lib/ADAA2:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA2:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA2:license", "LGPL v3.0 license");
		m->declare("aanl.lib/arctan:author", "Dario Sanfilippo");
		m->declare("aanl.lib/arctan:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/arctan:license", "LGPL v3.0 license");
		m->declare("aanl.lib/hardclip2:author", "Dario Sanfilippo");
		m->declare("aanl.lib/hardclip2:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/hardclip2:license", "LGPL v3.0 license");
		m->declare("aanl.lib/name", "Antialiased nonlinearities");
		m->declare("aanl.lib/version", "0.1");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("compile_options", "-a C:/Users/janka/Documents/Projects/Programming/JuceVSTs/clipping\Faust\faustMinimal.h -lang cpp -i -scn faustdsp -es 1 -mcd 16 -single -ftz 0");
		m->declare("filename", "FaustEffect.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "FaustEffect");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		float fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.0999985f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(0.0f);
		fEntry1 = FAUSTFLOAT(0.0f);
		fEntry2 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("FaustEffect");
		ui_interface->addNumEntry("clipping", &fEntry0, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addNumEntry("drive", &fEntry1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addNumEntry("squeeze", &fEntry2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = float(fEntry0);
		float fSlow1 = std::min<float>(20.0f * fSlow0, 1.0f);
		float fSlow2 = std::max<float>(1.1920929e-07f, 20.0f * mydsp_faustpower2_f(0.850000024f * fSlow0 + 0.150000006f));
		float fSlow3 = fConst1 * std::pow(10.0f, float(fEntry1));
		float fSlow4 = fConst1 * std::pow(10.0f, float(fEntry2));
		float fSlow5 = fSlow2 * std::atan(fSlow2);
		float fSlow6 = std::log(std::pow(fSlow2, 2.0f) + 1.0f);
		float fSlow7 = 1.0f - fSlow1;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fVec0[0] = fSlow2;
			fRec0[0] = fSlow3 + fConst2 * fRec0[1];
			fRec1[0] = fSlow4 + fConst2 * fRec1[1];
			float fTemp0 = float(input0[i0]) * fRec0[0] * fRec1[0];
			float fTemp1 = fSlow2 * fTemp0;
			fVec1[0] = fTemp1;
			float fTemp2 = fTemp1 - fVec1[1];
			float fThen0 = ((fSlow2 * fTemp0 * std::atan(fTemp1) + 0.5f * (std::log(std::pow(fVec1[1], 2.0f) + 1.0f) - std::log(std::pow(fTemp1, 2.0f) + 1.0f))) - fVec1[1] * std::atan(fVec1[1])) / fTemp2;
			float fElse0 = std::atan(0.5f * (fTemp1 + fVec1[1]));
			float fTemp3 = fSlow2 - fVec0[1];
			float fThen1 = (fSlow5 - (fVec0[1] * std::atan(fVec0[1]) + 0.5f * (fSlow6 - std::log(std::pow(fVec0[1], 2.0f) + 1.0f)))) / fTemp3;
			float fElse1 = std::atan(0.5f * (fSlow2 + fVec0[1]));
			float fTemp4 = fSlow1 * ((std::fabs(fTemp2) <= fConst3) ? fElse0 : fThen0) / ((std::fabs(fTemp3) <= fConst3) ? fElse1 : fThen1);
			float fTemp5 = fSlow7 * fTemp0;
			float fTemp6 = fTemp4 + fTemp5;
			fVec2[0] = fTemp6;
			float fTemp7 = std::pow(fTemp6 - fVec2[1], 2.0f);
			int iTemp8 = (fTemp6 <= 1.0f) & (fTemp6 >= -1.0f);
			float fTemp9 = float((fTemp6 > 0.0f) - (fTemp6 < 0.0f));
			float fTemp10 = 0.5f * std::pow(fTemp6, 2.0f);
			float fThen2 = fTemp6 * fTemp9 + -0.5f;
			int iTemp11 = (fVec2[1] <= 1.0f) & (fVec2[1] >= -1.0f);
			float fTemp12 = float((fVec2[1] > 0.0f) - (fVec2[1] < 0.0f));
			float fTemp13 = 0.5f * std::pow(fVec2[1], 2.0f);
			float fThen3 = fVec2[1] * fTemp12 + -0.5f;
			float fTemp14 = ((iTemp11) ? fTemp13 : fThen3);
			float fThen4 = fTemp12 * (fTemp13 + -0.166666672f);
			float fElse4 = 0.333333343f * std::pow(fVec2[1], 3.0f);
			float fTemp15 = ((iTemp11) ? fElse4 : fThen4);
			float fThen5 = fTemp9 * (fTemp10 + -0.166666672f);
			float fElse5 = 0.333333343f * std::pow(fTemp6, 3.0f);
			float fTemp16 = 2.0f * fVec2[1];
			float fThen6 = ((fTemp6 * (((iTemp8) ? fTemp10 : fThen2) - fTemp14) + fTemp15) - ((iTemp8) ? fElse5 : fThen5)) / fTemp7;
			float fElse6 = 0.5f * std::max<float>(-1.0f, std::min<float>(1.0f, 0.333333343f * (fTemp4 + fTemp5 + fTemp16)));
			int iTemp17 = (fVec2[2] <= 1.0f) & (fVec2[2] >= -1.0f);
			float fTemp18 = float((fVec2[2] > 0.0f) - (fVec2[2] < 0.0f));
			float fTemp19 = 0.5f * std::pow(fVec2[2], 2.0f);
			float fThen7 = fVec2[2] * fTemp18 + -0.5f;
			float fThen8 = fTemp18 * (fTemp19 + -0.166666672f);
			float fElse8 = 0.333333343f * std::pow(fVec2[2], 3.0f);
			float fThen9 = ((fTemp15 + fVec2[2] * (((iTemp17) ? fTemp19 : fThen7) - fTemp14)) - ((iTemp17) ? fElse8 : fThen8)) / std::pow(fVec2[2] - fVec2[1], 2.0f);
			float fElse9 = 0.5f * std::max<float>(-1.0f, std::min<float>(1.0f, 0.333333343f * (fTemp16 + fVec2[2])));
			output0[i0] = FAUSTFLOAT((((fTemp7 <= fConst3) ? fElse6 : fThen6) + ((std::pow(fVec2[1] - fVec2[2], 2.0f) <= fConst3) ? fElse9 : fThen9)) / fRec1[0]);
			fVec0[1] = fVec0[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
			fVec1[1] = fVec1[0];
			fVec2[2] = fVec2[1];
			fVec2[1] = fVec2[0];
		}
	}

};

// END-FAUSTDSP

#endif
