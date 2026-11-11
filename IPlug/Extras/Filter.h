//
// Tim Fan <fanchenhao2206@icloud.com>

#pragma once

#include "IPlugPlatform.h"
#include "SVF.h"

BEGIN_IPLUG_NAMESPACE

template <typename T>
class IFilter
{
public:
  IFilter() {}
  virtual void ProcessBlock(T** inputs, T** outputs, int nChans, int nFrames) {}
  virtual void Reset() {}
  virtual void SetSampleRate(double sampleRate)
  {
    mSampleRate = sampleRate;
  }

protected:
  double mSampleRate = 44100.;
};

template<typename T, int NC = 1>
class StateVariableFilter : public IFilter<T>
{
public:
  StateVariableFilter(
    typename SVF<T, NC>::EMode mode = SVF<T, NC>::kLowPass,
    double freq = 1000.0
  ) : mSVF(mode, freq)
  {}

  void SetFreqCPS(double freq)
  {
    mSVF.SetFreqCPS(freq);
  }

  void SetQ(double Q)
  {
    mSVF.SetQ(Q);
  }

  void SetGain(double gainDB)
  {
    mSVF.SetGain(gainDB);
  }

  void SetMode(typename SVF<T, NC>::EMode mode)
  {
    mSVF.SetMode(mode);
  }

  void SetSampleRate(double sampleRate) override
  {
    mSVF.SetSampleRate(sampleRate);
  }

  void ProcessBlock(T** inputs, T** outputs, int nChans, int nFrames) override
  {
    mSVF.ProcessBlock(inputs, outputs, nChans, nFrames);
  }

  void Reset() override
  {
    mSVF.Reset();
  }

private:
  SVF<T, NC> mSVF;
};

END_IPLUG_NAMESPACE

