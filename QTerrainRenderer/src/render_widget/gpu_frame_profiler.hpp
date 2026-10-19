#pragma once

#ifdef QTR_ENABLE_TRACY
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QPointer>
#include <array>
#include <cstdint>

namespace qtr
{
// One bounded query pool per GL context. Call only with that context current.
class GpuFrameProfiler : protected QOpenGLFunctions_3_3_Core
{
public:
  GpuFrameProfiler(QOpenGLContext *context, const void *renderer);
  ~GpuFrameProfiler();
  void begin();
  void end();

private:
  void collect();
  struct Slot
  {
    GLuint   begin = 0, end = 0;
    bool     pending = false;
    uint64_t connection = 0;
  };
  QPointer<QOpenGLContext> context;
  std::array<Slot, 8>      query_slots{};
  std::size_t              next_slot = 0;
  int                      active = -1;
  int                      counter_bits = 0;
  const char              *plot_name = nullptr;
  uint64_t                 samples = 0, skipped = 0;
};

class GpuFrameScope
{
public:
  explicit GpuFrameScope(GpuFrameProfiler *profiler) : profiler(profiler)
  {
    if (profiler)
      profiler->begin();
  }
  ~GpuFrameScope()
  {
    if (profiler)
      profiler->end();
  }
  GpuFrameScope(const GpuFrameScope &) = delete;
  GpuFrameScope &operator=(const GpuFrameScope &) = delete;

private:
  GpuFrameProfiler *profiler;
};
} // namespace qtr
#endif
