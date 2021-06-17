/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __PROSPER_STRUCTS_HPP__
#define __PROSPER_STRUCTS_HPP__

#include "prosper_definitions.hpp"
#include "prosper_enums.hpp"
#include <iglfw/glfw_window.h>
#include <limits>
#include <array>
#include <cinttypes>
#include <optional>
#include <string>
#include <cstring>
#include <map>

#undef max

namespace prosper
{
	struct DLLPROSPER Extent2D
	{
		Extent2D( uint32_t width_ = 0,
			uint32_t height_ = 0 )
			: width( width_ )
			, height( height_ )
		{}

		Extent2D( Extent2D const & rhs )
		{
			*reinterpret_cast<Extent2D*>(this) = rhs;
		}

		Extent2D & setWidth( uint32_t width_ )
		{
			width = width_;
			return *this;
		}

		Extent2D & setHeight( uint32_t height_ )
		{
			height = height_;
			return *this;
		}

		operator Extent2D const&() const
		{
			return *reinterpret_cast<const Extent2D*>( this );
		}

		operator Extent2D &()
		{
			return *reinterpret_cast<Extent2D*>( this );
		}

		bool operator==( Extent2D const& rhs ) const
		{
			return ( width == rhs.width )
				&& ( height == rhs.height );
		}

		bool operator!=( Extent2D const& rhs ) const
		{
			return !operator==( rhs );
		}

		uint32_t width;
		uint32_t height;
	};

	struct DLLPROSPER Extent3D
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t depth = 0;
	};

	struct DLLPROSPER Offset3D
	{
		Offset3D(int32_t x=0,int32_t y=0,int32_t z=0)
			: x{x},y{y},z{z}
		{}

		int32_t x;
		int32_t y;
		int32_t z;
	};

	class IImage;
	class IBuffer;
	class ISampler;
	class IImageView;
	namespace util
	{
		struct DLLPROSPER BufferCopy
		{
			DeviceSize srcOffset = 0;
			DeviceSize dstOffset = 0;
			DeviceSize size = 0;
		};

		struct DLLPROSPER ImageSubresourceRange
		{
			ImageSubresourceRange()=default;
			ImageSubresourceRange(uint32_t baseLayer,uint32_t layerCount,uint32_t baseMipmapLevel,uint32_t mipmapCount)
				: baseArrayLayer(baseLayer),layerCount(layerCount),baseMipLevel(baseMipmapLevel),levelCount(mipmapCount)
			{}
			ImageSubresourceRange(uint32_t baseLayer,uint32_t layerCount,uint32_t baseMipmapLevel)
				: ImageSubresourceRange(baseLayer,layerCount,baseMipmapLevel,1u)
			{}
			ImageSubresourceRange(uint32_t baseLayer,uint32_t layerCount)
				: ImageSubresourceRange(baseLayer,layerCount,0u,std::numeric_limits<uint32_t>::max())
			{}
			ImageSubresourceRange(uint32_t baseLayer)
				: ImageSubresourceRange(baseLayer,1u)
			{}
			uint32_t baseMipLevel = 0u;
			uint32_t levelCount = std::numeric_limits<uint32_t>::max();
			uint32_t baseArrayLayer = 0u;
			uint32_t layerCount = std::numeric_limits<uint32_t>::max();
		};

		struct DLLPROSPER ClearImageInfo
		{
			ImageSubresourceRange subresourceRange = {};
		};

		struct DLLPROSPER ImageSubresourceLayers
		{
			ImageSubresourceLayers(ImageAspectFlags aspectMask=ImageAspectFlags::ColorBit,uint32_t mipLevel=0,uint32_t baseArrayLayer=0,uint32_t layerCount=1)
				: aspectMask{aspectMask},mipLevel{mipLevel},baseArrayLayer{baseArrayLayer},layerCount{layerCount}
			{}
			ImageAspectFlags aspectMask = ImageAspectFlags::ColorBit;
			uint32_t mipLevel = 0;
			uint32_t baseArrayLayer = 0;
			uint32_t layerCount = 1;
		};

		struct DLLPROSPER CopyInfo
		{
			uint32_t width = std::numeric_limits<uint32_t>::max();
			uint32_t height = std::numeric_limits<uint32_t>::max();
			ImageSubresourceLayers srcSubresource = {ImageAspectFlags::ColorBit,0u,0u,1u};
			ImageSubresourceLayers dstSubresource = {ImageAspectFlags::ColorBit,0u,0u,1u};

			Offset3D srcOffset = Offset3D(0,0,0);
			Offset3D dstOffset = Offset3D(0,0,0);

			ImageLayout srcImageLayout = ImageLayout::TransferSrcOptimal;
			ImageLayout dstImageLayout = ImageLayout::TransferDstOptimal;
		};

		struct DLLPROSPER BufferImageCopyInfo
		{
			DeviceSize bufferOffset = 0ull;
			std::optional<uint32_t> width = {};
			std::optional<uint32_t> height = {};
			uint32_t mipLevel = 0u;
			uint32_t baseArrayLayer = 0u;
			uint32_t layerCount = 1u;
			ImageAspectFlags aspectMask = ImageAspectFlags::ColorBit;
			ImageLayout dstImageLayout = ImageLayout::TransferDstOptimal;
		};

		struct DLLPROSPER BlitInfo
		{
			ImageSubresourceLayers srcSubresourceLayer = {
				ImageAspectFlags::ColorBit,0u,0u,1u
			};
			ImageSubresourceLayers dstSubresourceLayer = {
				ImageAspectFlags::ColorBit,0u,0u,1u
			};

			std::array<int32_t,2> offsetSrc = {0,0};
			std::optional<Extent2D> extentsSrc = {};

			std::array<int32_t,2> offsetDst = {0,0};
			std::optional<Extent2D> extentsDst = {};
		};

		struct DLLPROSPER BufferBarrier
		{
			AccessFlags dstAccessMask;
			AccessFlags srcAccessMask;

			IBuffer *buffer;
			uint32_t dstQueueFamilyIndex;
			DeviceSize offset;
			DeviceSize size;
			uint32_t srcQueueFamilyIndex;

			BufferBarrier(
				AccessFlags in_source_access_mask,
				AccessFlags in_destination_access_mask,
				uint32_t in_src_queue_family_index,
				uint32_t in_dst_queue_family_index,
				IBuffer *in_buffer_ptr,
				DeviceSize in_offset,
				DeviceSize in_size
			)
				: srcAccessMask{in_source_access_mask},dstAccessMask{in_destination_access_mask},
				srcQueueFamilyIndex{in_src_queue_family_index},dstQueueFamilyIndex{in_dst_queue_family_index},
				buffer{in_buffer_ptr},offset{in_offset},size{in_size}
			{}
		};

		struct DLLPROSPER ImageBarrier
		{
			AccessFlags dstAccessMask;
			AccessFlags srcAccessMask;

			uint32_t dstQueueFamilyIndex;
			IImage *image;
			ImageLayout newLayout;
			ImageLayout oldLayout;
			uint32_t srcQueueFamilyIndex;
			ImageSubresourceRange subresourceRange;

			ImageBarrier(
				AccessFlags in_source_access_mask,
				AccessFlags in_destination_access_mask,
				ImageLayout in_old_layout,
				ImageLayout in_new_layout,
				uint32_t in_src_queue_family_index,
				uint32_t in_dst_queue_family_index,
				IImage *in_image_ptr,
				ImageSubresourceRange in_image_subresource_range
			)
				: srcAccessMask{in_source_access_mask},dstAccessMask{in_destination_access_mask},oldLayout{in_old_layout},
				newLayout{in_new_layout},srcQueueFamilyIndex{in_src_queue_family_index},dstQueueFamilyIndex{in_dst_queue_family_index},
				image{in_image_ptr},subresourceRange{in_image_subresource_range}
			{}
		};

		struct DLLPROSPER PipelineBarrierInfo
		{
			PipelineStageFlags srcStageMask = PipelineStageFlags::AllCommands;
			PipelineStageFlags dstStageMask = PipelineStageFlags::AllCommands;
			std::vector<BufferBarrier> bufferBarriers;
			std::vector<ImageBarrier> imageBarriers;

			PipelineBarrierInfo()=default;
			PipelineBarrierInfo &operator=(const PipelineBarrierInfo&)=delete;
		};

		struct DLLPROSPER BarrierImageLayout
		{
			BarrierImageLayout(PipelineStageFlags pipelineStageFlags,ImageLayout imageLayout,AccessFlags accessFlags);
			BarrierImageLayout()=default;
			PipelineStageFlags stageMask = {};
			ImageLayout layout = {};
			AccessFlags accessMask = {};
		};

		struct DLLPROSPER BufferBarrierInfo
		{
			AccessFlags srcAccessMask = {};
			AccessFlags dstAccessMask = {};
			DeviceSize offset = 0ull;
			DeviceSize size = std::numeric_limits<DeviceSize>::max();
		};

		struct DLLPROSPER ImageBarrierInfo
		{
			AccessFlags srcAccessMask = {};
			AccessFlags dstAccessMask = {};
			ImageLayout oldLayout = ImageLayout::General;
			ImageLayout newLayout = ImageLayout::General;
			uint32_t srcQueueFamilyIndex = QUEUE_FAMILY_IGNORED;
			uint32_t dstQueueFamilyIndex = QUEUE_FAMILY_IGNORED;

			ImageSubresourceRange subresourceRange = {};
		};

		struct DLLPROSPER ImageResolve
		{
			ImageSubresourceLayers src_subresource;
			Offset3D src_offset;
			ImageSubresourceLayers dst_subresource;
			Offset3D dst_offset;
			Extent3D extent;
		};

		struct DLLPROSPER SubresourceLayout
		{
			DeviceSize offset;
			DeviceSize size;
			DeviceSize row_pitch;
			DeviceSize array_pitch;
			DeviceSize depth_pitch;
		};

		struct DLLPROSPER ImageCreateInfo
		{
			ImageType type = ImageType::e2D;
			uint32_t width = 0u;
			uint32_t height = 0u;
			Format format = Format::R8G8B8A8_UNorm;
			uint32_t layers = 1u;
			ImageUsageFlags usage = ImageUsageFlags::ColorAttachmentBit;
			SampleCountFlags samples = SampleCountFlags::e1Bit;
			ImageTiling tiling = ImageTiling::Optimal;
			ImageLayout postCreateLayout = ImageLayout::ColorAttachmentOptimal;

			enum class Flags : uint32_t
			{
				None = 0u,
				Cubemap = 1u,
				ConcurrentSharing = Cubemap<<1u, // Exclusive sharing is used if this flag is not set
				FullMipmapChain = ConcurrentSharing<<1u,

				Sparse = FullMipmapChain<<1u,
				SparseAliasedResidency = Sparse<<1u, // Only has an effect if Sparse-flag is set

				AllocateDiscreteMemory = SparseAliasedResidency<<1u,
				DontAllocateMemory = AllocateDiscreteMemory<<1u,

				Srgb = DontAllocateMemory<<1u,
				NormalMap = Srgb<<1u
			};
			Flags flags = Flags::None;
			QueueFamilyFlags queueFamilyMask = QueueFamilyFlags::GraphicsBit;
			MemoryFeatureFlags memoryFeatures = MemoryFeatureFlags::GPUBulk;
		};

		struct DLLPROSPER TextureCreateInfo
		{
			TextureCreateInfo(IImageView &imgView,ISampler *smp=nullptr);
			TextureCreateInfo()=default;
			std::shared_ptr<ISampler> sampler = nullptr;
			std::shared_ptr<IImageView> imageView = nullptr;

			enum class Flags : uint32_t
			{
				None = 0u,
				Resolvable = 1u, // If this flag is set, a MSAATexture instead of a regular texture will be created, unless the image has a sample count of 1
				CreateImageViewForEachLayer = Resolvable<<1u
			};
			Flags flags = Flags::None;
		};

		struct DLLPROSPER ImageViewCreateInfo
		{
			std::optional<uint32_t> baseLayer = {};
			uint32_t levelCount = 1u;
			uint32_t baseMipmap = 0u;
			uint32_t mipmapLevels = std::numeric_limits<uint32_t>::max();
			Format format = Format::Unknown;
			ComponentSwizzle swizzleRed = ComponentSwizzle::R;
			ComponentSwizzle swizzleGreen = ComponentSwizzle::G;
			ComponentSwizzle swizzleBlue = ComponentSwizzle::B;
			ComponentSwizzle swizzleAlpha = ComponentSwizzle::A;
		};

		struct DLLPROSPER SamplerCreateInfo
		{
			Filter minFilter = Filter::Linear;
			Filter magFilter = Filter::Linear;
			SamplerMipmapMode mipmapMode = SamplerMipmapMode::Linear;
			SamplerAddressMode addressModeU = SamplerAddressMode::Repeat;
			SamplerAddressMode addressModeV = SamplerAddressMode::Repeat;
			SamplerAddressMode addressModeW = SamplerAddressMode::Repeat;
			float mipLodBias = 0.f;
			float maxAnisotropy = std::numeric_limits<float>::max();
			bool compareEnable = false;
			CompareOp compareOp = CompareOp::Less;
			float minLod = 0.f;
			float maxLod = std::numeric_limits<float>::max();
			BorderColor borderColor = BorderColor::FloatTransparentBlack;
			// bool useUnnormalizedCoordinates = false;
		};

		struct DLLPROSPER RenderTargetCreateInfo
		{
			bool useLayerFramebuffers = false;
		};

		struct DLLPROSPER RenderPassCreateInfo
		{
			struct DLLPROSPER AttachmentInfo
			{
				AttachmentInfo(
					prosper::Format format=prosper::Format::R8G8B8A8_UNorm,prosper::ImageLayout initialLayout=prosper::ImageLayout::ColorAttachmentOptimal,
					prosper::AttachmentLoadOp loadOp=prosper::AttachmentLoadOp::DontCare,prosper::AttachmentStoreOp storeOp=prosper::AttachmentStoreOp::Store,
					prosper::SampleCountFlags sampleCount=prosper::SampleCountFlags::e1Bit,prosper::ImageLayout finalLayout=prosper::ImageLayout::ShaderReadOnlyOptimal
				);
				bool operator==(const AttachmentInfo &other) const;
				bool operator!=(const AttachmentInfo &other) const;
				prosper::Format format = prosper::Format::R8G8B8A8_UNorm;
				prosper::SampleCountFlags sampleCount = prosper::SampleCountFlags::e1Bit;
				prosper::AttachmentLoadOp loadOp = prosper::AttachmentLoadOp::DontCare;
				prosper::AttachmentStoreOp storeOp = prosper::AttachmentStoreOp::Store;
				prosper::ImageLayout initialLayout = prosper::ImageLayout::ColorAttachmentOptimal;
				prosper::ImageLayout finalLayout = prosper::ImageLayout::ShaderReadOnlyOptimal;
			};
			struct DLLPROSPER SubPass
			{
				struct DLLPROSPER Dependency
				{
					Dependency(
						std::size_t sourceSubPassId,std::size_t destinationSubPassId,
						prosper::PipelineStageFlags sourceStageMask,prosper::PipelineStageFlags destinationStageMask,
						prosper::AccessFlags sourceAccessMask,prosper::AccessFlags destinationAccessMask
					);
					bool operator==(const Dependency &other) const;
					bool operator!=(const Dependency &other) const;
					std::size_t sourceSubPassId;
					std::size_t destinationSubPassId;
					prosper::PipelineStageFlags sourceStageMask;
					prosper::PipelineStageFlags destinationStageMask;
					prosper::AccessFlags sourceAccessMask;
					prosper::AccessFlags destinationAccessMask;
				};
				SubPass(const std::vector<std::size_t> &colorAttachments={},bool useDepthStencilAttachment=false,std::vector<Dependency> dependencies={});
				bool operator==(const SubPass &other) const;
				bool operator!=(const SubPass &other) const;
				std::vector<std::size_t> colorAttachments = {};
				bool useDepthStencilAttachment = false;
				std::vector<Dependency> dependencies = {};
			};
			RenderPassCreateInfo(const std::vector<AttachmentInfo> &attachments={},const std::vector<SubPass> &subPasses={});
			bool operator==(const RenderPassCreateInfo &other) const;
			bool operator!=(const RenderPassCreateInfo &other) const;
			std::vector<AttachmentInfo> attachments;
			std::vector<SubPass> subPasses;
		};
	};

	class DLLPROSPER DescriptorSetCreateInfo
	{
	public:
		static std::unique_ptr<DescriptorSetCreateInfo> Create()
		{
			std::unique_ptr<DescriptorSetCreateInfo> result_ptr(nullptr,
				std::default_delete<DescriptorSetCreateInfo>() );

			result_ptr.reset(
				new DescriptorSetCreateInfo()
			);

			return result_ptr;
		}
		struct DLLPROSPER Binding
		{
			Binding()=default;
			Binding(const Binding &other)=default;
			Binding(
				uint32_t descriptorArraySize,
				DescriptorType descriptorType,
				ShaderStageFlags stageFlags,
				const ISampler * const* immutableSamplerPtrs,
				DescriptorBindingFlags flags
			)
				: descriptorArraySize{descriptorArraySize},descriptorType{descriptorType},
				stageFlags{stageFlags},flags{flags}
			{
				if(immutableSamplerPtrs != nullptr)
				{
					for(uint32_t n_sampler = 0;
						n_sampler < descriptorArraySize;
						++n_sampler)
					{
						immutableSamplers.push_back(immutableSamplerPtrs[n_sampler]);
					}
				}
			}
			uint32_t descriptorArraySize = 0;
			DescriptorType descriptorType = DescriptorType::Unknown;
			DescriptorBindingFlags flags {};
			std::vector<const ISampler*> immutableSamplers {};
			ShaderStageFlags stageFlags {};
			bool operator==(const Binding &binding) const
			{
				return binding.descriptorArraySize == descriptorArraySize &&
					binding.descriptorType == descriptorType &&
					binding.flags == flags &&
					binding.immutableSamplers == immutableSamplers &&
					binding.stageFlags == stageFlags;
			}
			bool operator!=(const Binding &binding) const {return !operator==(binding);}
		};
		DescriptorSetCreateInfo(const DescriptorSetCreateInfo &other);
		DescriptorSetCreateInfo &operator=(const DescriptorSetCreateInfo &other);
		DescriptorSetCreateInfo(DescriptorSetCreateInfo &&other);
		DescriptorSetCreateInfo &operator=(DescriptorSetCreateInfo &&other);

		bool GetBindingPropertiesByBindingIndex(
			uint32_t bindingIndex,
			DescriptorType *outOptDescriptorType=nullptr,
			uint32_t *outOptDescriptorArraySize=nullptr,
			ShaderStageFlags *outOptStageFlags=nullptr,
			bool *outOptImmutableSamplersEnabled=nullptr,
			DescriptorBindingFlags *outOptFlags=nullptr
		) const;
		bool GetBindingPropertiesByIndexNumber(
			uint32_t nBinding,
			uint32_t *out_opt_binding_index_ptr=nullptr,
			DescriptorType *outOptDescriptorType=nullptr,
			uint32_t *outOptDescriptorArraySize=nullptr,
			ShaderStageFlags *outOptStageFlags=nullptr,
			bool *outOptImmutableSamplersEnabled=nullptr,
			DescriptorBindingFlags *outOptFlags=nullptr
		);
		bool AddBinding(uint32_t                               in_binding_index,
			DescriptorType                  in_descriptor_type,
			uint32_t                               in_descriptor_array_size,
			ShaderStageFlags                in_stage_flags,
			const DescriptorBindingFlags&   in_flags                         = DescriptorBindingFlags::None,
			const ISampler* const*           in_opt_immutable_sampler_ptr_ptr = nullptr);
		uint32_t GetBindingCount() const {return static_cast<uint32_t>(m_bindings.size());}
	private:
		using BindingIndexToBindingMap = std::map<BindingIndex,Binding>;

		DescriptorSetCreateInfo()=default;

		BindingIndexToBindingMap m_bindings {};

		uint32_t m_numVariableDescriptorCountBinding = std::numeric_limits<uint32_t>::max();
		uint32_t m_variableDescriptorCountBindingSize = 0;
	};

	struct DLLPROSPER SpecializationConstant
	{
		SpecializationConstant(
			uint32_t constantId,
			uint32_t numBytes,
			uint32_t startOffset
		)
			: constantId{constantId},numBytes{numBytes},
			startOffset{startOffset}
		{}
		uint32_t constantId;
		uint32_t numBytes;
		uint32_t startOffset;
	};

	struct DLLPROSPER PushConstantRange
	{
		PushConstantRange(
			uint32_t offset,
			uint32_t size,
			ShaderStageFlags stages
		)
			: offset{offset},size{size},
			stages{stages}
		{}
		uint32_t offset;
		uint32_t size;
		ShaderStageFlags stages;

		bool operator==(const PushConstantRange& in) const
		{
			return in.offset == offset &&
				in.size == size &&
				in.stages == stages;
		}
		bool operator!=(const PushConstantRange& in) const {return !operator==(in);}
	};

	class ShaderStageProgram;
	class DLLPROSPER ShaderModule
	{
	public:
		ShaderModule(
			const std::shared_ptr<ShaderStageProgram> &shaderStageProgram,
			const std::string&          in_opt_cs_entrypoint_name,
			const std::string&          in_opt_fs_entrypoint_name,
			const std::string&          in_opt_gs_entrypoint_name,
			const std::string&          in_opt_tc_entrypoint_name,
			const std::string&          in_opt_te_entrypoint_name,
			const std::string&          in_opt_vs_entrypoint_name
		);
		/** Destructor. Releases internally maintained Vulkan shader module instance. */
		virtual ~ShaderModule()=default;

		const std::string& GetCSEntrypointName() const
		{
			return m_csEntrypointName;
		}

		const std::string& GetFSEntrypointName() const
		{
			return m_fsEntrypointName;
		}

		const std::string& GetGLSLSourceCode() const
		{
			return m_glslSourceCode;
		}
		const std::string& GetGSEntrypointName() const
		{
			return m_gsEntrypointName;
		}
		const std::string& GetTCEntrypointName() const
		{
			return m_tcEntrypointName;
		}
		const std::string& GetTEEntrypointName() const
		{
			return m_teEntrypointName;
		}
		const std::string& GetVSEntrypointName() const
		{
			return m_vsEntrypointName;
		}
		const ShaderStageProgram *GetShaderStageProgram() const {return m_shaderStageProgram.get();}

	private:
		ShaderModule           (const ShaderModule&);
		ShaderModule& operator=(const ShaderModule&);

		std::string m_csEntrypointName;
		std::string m_fsEntrypointName;
		std::string m_gsEntrypointName;
		std::string m_tcEntrypointName;
		std::string m_teEntrypointName;
		std::string m_vsEntrypointName;
		std::string m_glslSourceCode;
		std::shared_ptr<ShaderStageProgram> m_shaderStageProgram {};
	};

	struct DLLPROSPER ShaderModuleStageEntryPoint
	{
		std::string name = "";
		std::unique_ptr<ShaderModule> shader_module_owned_ptr = nullptr;
		ShaderModule *shader_module_ptr = nullptr;
		ShaderStage stage = ShaderStage::Unknown;

		ShaderModuleStageEntryPoint()=default;
		~ShaderModuleStageEntryPoint()=default;

		ShaderModuleStageEntryPoint(const ShaderModuleStageEntryPoint& in);

		ShaderModuleStageEntryPoint(const std::string&    in_name,
			ShaderModule*         in_shader_module_ptr,
			ShaderStage           in_stage);
		ShaderModuleStageEntryPoint(const std::string&    in_name,
			std::unique_ptr<ShaderModule> in_shader_module_ptr,
			ShaderStage           in_stage);

		ShaderModuleStageEntryPoint& operator=(const ShaderModuleStageEntryPoint&);
	};

	struct DLLPROSPER VertexInputAttribute
	{
		Format format = Format::Unknown;
		uint32_t location = std::numeric_limits<uint32_t>::max();
		uint32_t offsetInBytes = std::numeric_limits<uint32_t>::max();

		VertexInputAttribute()=default;
		VertexInputAttribute(uint32_t location,
			Format format,
			uint32_t offset
		)
			: location{location},format{format},offsetInBytes{offset}
		{}
	};

	struct DLLPROSPER StencilOpState
	{
		StencilOp failOp;
		StencilOp passOp;
		StencilOp depthFailOp;
		CompareOp compareOp;
		uint32_t compareMask;
		uint32_t writeMask;
		uint32_t reference;
	};
	
	struct DLLPROSPER ImageFormatPropertiesQuery
	{
		ImageCreateFlags createFlags;
		Format format;
		ImageType imageType;
		ImageTiling tiling;
		ImageUsageFlags usageFlags;
	};

	struct DLLPROSPER Offset2D
	{
		Offset2D( int32_t x_ = 0,
			int32_t y_ = 0 )
			: x( x_ )
			, y( y_ )
		{}

		Offset2D( Offset2D const & rhs )
		{
			*reinterpret_cast<Offset2D*>(this) = rhs;
		}

		Offset2D & setX( int32_t x_ )
		{
			x = x_;
			return *this;
		}

		Offset2D & setY( int32_t y_ )
		{
			y = y_;
			return *this;
		}

		operator Offset2D const&() const
		{
			return *reinterpret_cast<const Offset2D*>( this );
		}

		operator Offset2D &()
		{
			return *reinterpret_cast<Offset2D*>( this );
		}

		bool operator==( Offset2D const& rhs ) const
		{
			return ( x == rhs.x )
				&& ( y == rhs.y );
		}

		bool operator!=( Offset2D const& rhs ) const
		{
			return !operator==( rhs );
		}

		int32_t x;
		int32_t y;
	};

	struct DLLPROSPER Rect2D
	{
		Rect2D( Offset2D offset_ = Offset2D(),
			Extent2D extent_ = Extent2D() )
			: offset( offset_ )
			, extent( extent_ )
		{}

		Rect2D( Rect2D const & rhs )
		{
			*reinterpret_cast<Rect2D*>(this) = rhs;
		}

		Rect2D & setOffset( Offset2D offset_ )
		{
			offset = offset_;
			return *this;
		}

		Rect2D & setExtent( Extent2D extent_ )
		{
			extent = extent_;
			return *this;
		}

		operator Rect2D const&() const
		{
			return *reinterpret_cast<const Rect2D*>( this );
		}

		operator Rect2D &()
		{
			return *reinterpret_cast<Rect2D*>( this );
		}

		bool operator==( Rect2D const& rhs ) const
		{
			return ( offset == rhs.offset )
				&& ( extent == rhs.extent );
		}

		bool operator!=( Rect2D const& rhs ) const
		{
			return !operator==( rhs );
		}

		Offset2D offset;
		Extent2D extent;
	};

	struct DLLPROSPER Viewport
	{
		float x;
		float y;
		float width;
		float height;
		float minDepth;
		float maxDepth;
	};

	union DLLPROSPER ClearColorValue
	{
		ClearColorValue( const std::array<float,4>& float32_ = { { 0 } } )
		{
			memcpy( float32, float32_.data(), 4 * sizeof( float ) );
		}

		ClearColorValue( const std::array<int32_t,4>& int32_ )
		{
			memcpy( int32, int32_.data(), 4 * sizeof( int32_t ) );
		}

		ClearColorValue( const std::array<uint32_t,4>& uint32_ )
		{
			memcpy( uint32, uint32_.data(), 4 * sizeof( uint32_t ) );
		}


		ClearColorValue & setFloat32( std::array<float,4> float32_ )
		{
			memcpy( float32, float32_.data(), 4 * sizeof( float ) );
			return *this;
		}

		ClearColorValue & setInt32( std::array<int32_t,4> int32_ )
		{
			memcpy( int32, int32_.data(), 4 * sizeof( int32_t ) );
			return *this;
		}

		ClearColorValue & setUint32( std::array<uint32_t,4> uint32_ )
		{
			memcpy( uint32, uint32_.data(), 4 * sizeof( uint32_t ) );
			return *this;
		}
		operator ClearColorValue const&() const
		{
			return *reinterpret_cast<const ClearColorValue*>(this);
		}

		operator ClearColorValue &()
		{
			return *reinterpret_cast<ClearColorValue*>(this);
		}

		float float32[4];
		int32_t int32[4];
		uint32_t uint32[4];
	};

	struct DLLPROSPER ClearDepthStencilValue
	{
		ClearDepthStencilValue( float depth_ = 0,
			uint32_t stencil_ = 0 )
			: depth( depth_ )
			, stencil( stencil_ )
		{}

		ClearDepthStencilValue( ClearDepthStencilValue const & rhs )
		{
			*reinterpret_cast<ClearDepthStencilValue*>(this) = rhs;
		}

		ClearDepthStencilValue & setDepth( float depth_ )
		{
			depth = depth_;
			return *this;
		}

		ClearDepthStencilValue & setStencil( uint32_t stencil_ )
		{
			stencil = stencil_;
			return *this;
		}

		operator ClearDepthStencilValue const&() const
		{
			return *reinterpret_cast<const ClearDepthStencilValue*>( this );
		}

		operator ClearDepthStencilValue &()
		{
			return *reinterpret_cast<ClearDepthStencilValue*>( this );
		}

		bool operator==( ClearDepthStencilValue const& rhs ) const
		{
			return ( depth == rhs.depth )
				&& ( stencil == rhs.stencil );
		}

		bool operator!=( ClearDepthStencilValue const& rhs ) const
		{
			return !operator==( rhs );
		}

		float depth;
		uint32_t stencil;
	};

	union DLLPROSPER ClearValue
	{
		ClearValue( ClearColorValue color_ = ClearColorValue() )
		{
			color = color_;
		}

		ClearValue( ClearDepthStencilValue depthStencil_ )
		{
			depthStencil = depthStencil_;
		}
		ClearValue(const ClearValue &cv)
			: color{cv.color}
		{}
		ClearValue &operator=(const ClearValue &other)
		{
			color = other.color;
			depthStencil = other.depthStencil;
			return *this;
		}

		ClearValue & setColor( ClearColorValue color_ )
		{
			color = color_;
			return *this;
		}

		ClearValue & setDepthStencil( ClearDepthStencilValue depthStencil_ )
		{
			depthStencil = depthStencil_;
			return *this;
		}
		operator ClearValue const&() const
		{
			return *reinterpret_cast<const ClearValue*>(this);
		}

		operator ClearValue &()
		{
			return *reinterpret_cast<ClearValue*>(this);
		}

		ClearColorValue color;
		ClearDepthStencilValue depthStencil;
	};

	struct DLLPROSPER IndexBufferInfo
	{
		std::shared_ptr<prosper::IBuffer> buffer = nullptr;
		prosper::IndexType indexType = prosper::IndexType::UInt16;
		prosper::DeviceSize offset = 0;
	};
	
	enum class Vendor : uint32_t;
	namespace util
	{
		struct DLLPROSPER VendorDeviceInfo
		{
			std::string apiVersion;
			std::string deviceName;
			PhysicalDeviceType deviceType;
			uint32_t deviceId;
			std::string driverVersion;
			Vendor vendor;
		};

		struct DLLPROSPER PhysicalDeviceMemoryProperties
		{
			std::vector<DeviceSize> heapSizes;
		};
	};

	using PipelineID = uint32_t;
	using SubPassID = uint32_t;
	using SubPassAttachmentID = uint32_t;

	struct DLLPROSPER WindowSettings
		: public GLFW::WindowCreationInfo
	{
		bool windowedMode = true;
		prosper::PresentModeKHR presentMode = prosper::PresentModeKHR::Immediate;
	};
};

#endif
