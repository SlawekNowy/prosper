/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "stdafx_prosper.h"
#include "shader/prosper_shader_manager.hpp"
#include "prosper_context.hpp"
#include "shader/prosper_shader.hpp"
#include <sharedutils/util_shaderinfo.hpp>
#include <sharedutils/util_string.h>
#include <iostream>

prosper::ShaderManager::ShaderManager(IPrContext &context)
	: ContextObject(context)
{}
util::WeakHandle<::util::ShaderInfo> prosper::ShaderManager::PreRegisterShader(const std::string &identifier)
{
	auto lidentifier = identifier;
	ustring::to_lower(lidentifier);
	auto it = m_shaderInfo.find(lidentifier);
	if(it != m_shaderInfo.end())
		return it->second;
	auto shaderInfo = std::make_shared<::util::ShaderInfo>(lidentifier);
	it = m_shaderInfo.insert(std::make_pair(lidentifier,shaderInfo)).first;
	return it->second;
}

::util::WeakHandle<prosper::Shader> prosper::ShaderManager::LoadShader(const std::string &identifier)
{
	auto it = m_shaders.find(identifier);
	if(it != m_shaders.end())
		return it->second;
	auto itFactory = m_shaderFactories.find(identifier);
	if(itFactory == m_shaderFactories.end())
		return {};
	auto wpShaderInfo = PreRegisterShader(identifier);
	auto &factory = itFactory->second;
	auto bExternalOwnership = false;
	auto *ptrShader = factory(GetContext(),identifier,bExternalOwnership);
	std::shared_ptr<Shader> shader = nullptr;
	if(bExternalOwnership == false)
		shader = std::shared_ptr<Shader>(ptrShader,[](Shader *shader) {shader->Release();});
	else
		shader = std::shared_ptr<Shader>(ptrShader,[](Shader *shader) {shader->Release(false);});
	m_shaders[identifier] = shader;
	auto wpShader = ::util::WeakHandle<Shader>(shader);
	wpShaderInfo.get()->SetShader(std::make_shared<::util::WeakHandle<Shader>>(wpShader));
	shader->Initialize();
	return wpShader;
}

void prosper::ShaderManager::RegisterShader(const std::string &identifier,const std::function<Shader*(IPrContext&,const std::string&,bool&)> &fFactory)
{
	if(GetContext().IsValidationEnabled())
		std::cout<<"[PR] Registering shader '"<<identifier<<"'..."<<std::endl;
	auto wpShaderInfo = PreRegisterShader(identifier);
	auto lidentifier = identifier;
	ustring::to_lower(lidentifier);

	// The shader will be initialized lazily (whenever it's accessed for the first time)
	m_shaderFactories[lidentifier] = fFactory;
}
void prosper::ShaderManager::RegisterShader(const std::string &identifier,const std::function<Shader*(IPrContext&,const std::string&)> &fFactory)
{
	RegisterShader(identifier,[fFactory](IPrContext &context,const std::string &identifier,bool &bExternalOwnership) {
		bExternalOwnership = false;
		return fFactory(context,identifier);
	});
}
util::WeakHandle<prosper::Shader> prosper::ShaderManager::GetShader(const std::string &identifier) const
{
	return const_cast<ShaderManager*>(this)->LoadShader(identifier);
	/*auto lidentifier = identifier;
	auto it = m_shaders.find(lidentifier);
	return (it != m_shaders.end()) ? ::util::WeakHandle<Shader>(it->second) : ::util::WeakHandle<Shader>();*/
}
const std::unordered_map<std::string,std::shared_ptr<prosper::Shader>> &prosper::ShaderManager::GetShaders() const {return m_shaders;}
bool prosper::ShaderManager::RemoveShader(Shader &shader)
{
	auto it = std::find_if(m_shaders.begin(),m_shaders.end(),[&shader](const std::pair<std::string,std::shared_ptr<Shader>> &pair) {
		return pair.second.get() == &shader;
	});
	if(it == m_shaders.end())
		return false;
	m_shaders.erase(it);
	return true;
}
