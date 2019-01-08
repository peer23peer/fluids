// MIT License
//
// Copyright (c) 2019 Jelle Spijker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef LIBFLUIDS_STATE_H
#define LIBFLUIDS_STATE_H

#include <memory>

#include "Units.h"

namespace Fluids {

class State {
public:
  State();
  State(const State &other);

  State &operator=(const State &other);

  virtual ~State() = default;

  virtual const std::shared_ptr<quantity<si::velocity>> &Get_Speed() const;
  virtual void Set_Speed(const std::shared_ptr<quantity<si::velocity>> &speed);

  virtual const std::shared_ptr<quantity<si::pressure>> &Get_Static_pressure() const;
  virtual void Set_Static_pressure(const std::shared_ptr<quantity<si::pressure>> &static_pressure);

protected:
  std::shared_ptr<quantity<si::pressure>> m_static_pressure;
  std::shared_ptr<quantity<si::velocity>> m_speed;

};

}

#endif //LIBFLUIDS_STATE_H