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

#ifndef FLUIDS_SYSTEM_H
#define FLUIDS_SYSTEM_H

#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <Eigen/Core>
#include <random>

#include "Liquid.h"
#include "FluidComponents.h"

namespace Fluids {

typedef boost::adjacency_list<boost::listS,
                              boost::listS,
                              boost::bidirectionalS,
                              std::shared_ptr<Liquid>,
                              std::shared_ptr<FluidComponents>> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;

typedef std::vector<std::shared_ptr<quantity<si::velocity>>> shared_velocity_vector;
typedef std::vector<std::shared_ptr<quantity<si::pressure>>> shared_pressure_vector;
typedef std::vector<std::shared_ptr<quantity<si::volumetric_flow>>> shared_volumetric_flow_vector;

class System {
public:
  System();
  System(Liquid liquid, size_t num_vertices);

  void add_FluidComponent(const std::shared_ptr<FluidComponents> &component,
                          const size_t &vertex_u,
                          const size_t &vertex_v);

  std::shared_ptr<Liquid> &Get_Liquid(const size_t &vertex_u);
  std::shared_ptr<FluidComponents> &Get_Component(const size_t &vertex_u, const size_t &vertex_v);

  void Initialize();

  void Set_Known_Speed(const size_t &vertex_u, const quantity<si::velocity> &speed);
  void Set_Known_Static_Pressure(const size_t &vertex_u, const quantity<si::pressure> &pressure);

  size_t n_unknowns();
  Eigen::VectorXd Get_Initial_vector();

  const shared_velocity_vector &Get_Known_speeds() const;
  const shared_velocity_vector &Get_Unknown_speeds() const;
  const shared_pressure_vector &Get_Known_static_pressures() const;
  const shared_pressure_vector &Get_Unknown_static_pressures() const;
  const shared_volumetric_flow_vector &Get_Unknown_volumetric_flow() const;
  const shared_volumetric_flow_vector &Get_Known_volumetric_flow() const;

  const Eigen::VectorXd Get_Return_vec() const;
  const Graph &Get_Graph() const;

private:
  Graph m_graph;
  shared_velocity_vector m_known_speeds;
  shared_velocity_vector m_unknown_speeds;
  shared_pressure_vector m_known_static_pressures;
  shared_pressure_vector m_unknown_static_pressures;
  shared_volumetric_flow_vector m_known_volumetric_flows;
  shared_volumetric_flow_vector m_unknown_volumetric_flows;

  const Eigen::VectorXd Get_Bernoulli_vec() const;
  const Eigen::VectorXd Get_massflow_vec() const;

  /// Is value present in vector
  /// \tparam T type of values in vector
  /// \param vec vector
  /// \param value value to check if it is in vector
  /// \return true if value is present in vector
  template<typename T>
  static bool in_vector(const std::vector<T> &vec, const T &value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
  };

  /// Set a known value for the system
  /// \tparam T
  /// \param p
  /// \param value
  /// \param known
  /// \param unknown
  template<typename T>
  void Set_Known(const std::shared_ptr<T> &p,
                 const T &value,
                 std::vector<std::shared_ptr<T>> &known,
                 std::vector<std::shared_ptr<T>> &unknown) {
    *p = value;
    if (!in_vector<std::shared_ptr<T>>(known, p)) {
      known.push_back(p);
    }
    if (in_vector<std::shared_ptr<T>>(unknown, p)) {
      unknown.erase(std::remove(unknown.begin(), unknown.end(), p), unknown.end());
    }
  }

  template<typename T>
  void initial_values(const std::vector<std::shared_ptr<quantity<T>>> &vec,
                      const T qty,
                      Eigen::VectorXd &initial_vec,
                      size_t start,
                      size_t end,
                      double low,
                      double high) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    if (!vec.empty()) {
      quantity<T> avg_known_system_value = 0.0 * qty;
      for (auto&& value : vec) {
        avg_known_system_value += *value;
      }
      avg_known_system_value /= vec.size();
      for (size_t k = start; k < end; ++k) {
        initial_vec(k) = avg_known_system_value.value();
      }
    } else {
      std::uniform_real_distribution<> dis(low, high);
      for (size_t k = start; k < end; ++k) {
        initial_vec(k) = dis(gen);
      }
    }
  };
};

}

#endif //FLUIDS_SYSTEM_H
