#pragma once

#include<set>
#include<memory>

template<typename TYPE>
typename std::set<std::shared_ptr<TYPE>>::iterator setOfSharedFind(std::set<std::shared_ptr<TYPE>> const&s,TYPE const&val){
  return std::find_if(s.begin(),s.end(),
      [&](std::shared_ptr<TYPE> const & instance) {return *instance == val;});
}

template<typename TYPE>
typename std::set<std::weak_ptr<TYPE>>::iterator setOfWeakFind(std::set<std::weak_ptr<TYPE>> const&s,TYPE const&val){
  return std::find_if(s.begin(),s.end(),
      [&](std::weak_ptr<TYPE> const & instance) {return *instance.lock() == val;});
}

template<typename TYPE>
typename std::set<std::weak_ptr<TYPE>,std::owner_less<std::weak_ptr<TYPE>>>::iterator setOfWeakFind(std::set<std::weak_ptr<TYPE>,std::owner_less<std::weak_ptr<TYPE>>> const&s,TYPE const&val){
  return std::find_if(s.begin(),s.end(),
      [&](std::weak_ptr<TYPE> const & instance) {return *instance.lock() == val;});
}

